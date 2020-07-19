#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtQuickWidgets/QtQuickWidgets>
#include "Packet.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <QGeoCoordinate>
#include <atomic>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->quickWidget->rootContext()->setContextProperty("mapMarker",&mapMarker);
    ui->quickWidget->setSource(QUrl("../PTZVisulizer/main.qml"));
    this->connected = ConnectionStatus::Off;



    connect(ui->SendPushbutton,SIGNAL(clicked(bool)),this,SLOT(sendCustomePacket()));
    connect(ui->connectPushButton,SIGNAL(clicked(bool)),this,SLOT(connectButtonStateMachine()));
    connect(ui->updatePTZs,SIGNAL(clicked(bool)),this ,SLOT(retrevePTZsInformation()));
   // connect(ui->addMarker, SIGNAL(clicked(bool)), this , SLOT(addMarker()));
    connect(ui->RefreshRate,SIGNAL(editingFinished()),this,SLOT(updateRefreshRate()));
    connect(ui->autoModeToggle,SIGNAL(clicked(bool)),this ,SLOT(toggleAutoMode()));

    this->refreshRateMS = 100;
    this->autoRefreshThread = std::thread(&MainWindow::autoMode,this);


}

void MainWindow::connectButtonStateMachine(){
    if(this->connected == ConnectionStatus::Off){
        if(connectToServer() < 0) return ;

        ui->connectPushButton->setText("Disconnect");
        this->connected = ConnectionStatus::On;
        initDrone();
        initPTZs();

        this->autoModeState.store(On);
        //this->autoRefreshThread = std::thread(&MainWindow::autoMode,this);
    }
    else {
        if(disconnectFromServer() < 0) return ;
        this->connected = ConnectionStatus::Off;
        ui->connectPushButton->setText("Connect");
        deletePTZs();
        this->autoModeState.store(Off);
    }
}

int MainWindow::disconnectFromServer(){
    if(this->connected == ConnectionStatus::On)
        return ::close(this->fdSocket);
    log("Threre is no connection to cut",true);
    return -1;
}
int MainWindow::connectToServer(){
    int fd ;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if(ui->IPAddress->text().isEmpty() || ui->PortNumber->text().isEmpty()){
        log("Please fill IP:Port " , true);
        return -1;
    }
    log("Connecting to " + ui->IPAddress->text() + ":" + ui->PortNumber->text() + ".",false);
     struct sockaddr_in remote;
     remote.sin_family = AF_INET;
     remote.sin_port = htons(ui->PortNumber->text().toInt());
     if(inet_pton(AF_INET, ui->IPAddress->text().toStdString().c_str(), &remote.sin_addr) < 0 ){
        log("Invalide remote host address",true);
        return -1;
     }


     fd = socket(AF_INET,SOCK_STREAM,0);
     if(fd < 0 ){
         log("Cant create socket ", true);
         return -1;
     }

     setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

     if(::connect(fd, (void*)&remote, sizeof(remote)) < 0){
        log("Connection was refused" , true);
        return -1;
     }



    this->fdSocket = fd;
    log("Connected ...." ,false);
}
Packet::pktFeedback MainWindow::sendPacket(Packet::pktCommand * cmd){
    if(this->connected != ConnectionStatus::On){
        log("Cant send packet before connecting to the server ",true);
        return Packet::pktFeedback();
    }

    Packet::pktFeedback fed;
    send(this->fdSocket,cmd, sizeof(Packet::pktCommand),0);
    read(this->fdSocket,&fed,sizeof(fed));

    return fed;

}


void MainWindow::sendCustomePacket(){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    if(this->connected == ConnectionStatus::Off){
        log("Please Connect to the Simulator First ",true);
        return;

    }

    if(ui->CpacketCommand->text().isEmpty() || ui->CPacketAction->text().isEmpty() || ui->CPacketPeripheral->text().isEmpty()
            || ui->CPacketPeriphFunction->text().isEmpty()){
        log("Please Fill the structure ",true);
        return;
    }


    cmd.cmd =(uint8_t) (ui->CpacketCommand->text().toInt() & 0xff);
    cmd.peripheral = (uint8_t) (ui->CPacketPeripheral->text().toInt() & 0xff);
    cmd.peripheral_function = (uint8_t) (ui->CPacketPeriphFunction->text().toInt() & 0xff);
    cmd.action = (uint8_t) (ui->CPacketAction->text().toInt() & 0xff);

    if(ui->CPacketTime->text().isEmpty()){
        log("time stamp is empty , use current time instead",false);
        cmd.time_stamp = (uint64_t)(QDateTime::currentSecsSinceEpoch());
    }
    else  cmd.time_stamp = (uint8_t) (ui->CPacketTime->text().toULongLong());


    fed = sendPacket(&cmd);
    log("Sending Packet ... ",false);

    ui->FPacketFeedback->setText(QString::number((int)fed.feedback));
    ui->FPacketCommand->setText(QString::number((int)fed.cmd));
    ui->FPacketPeripheral->setText(QString::number((int)fed.peripheral));
    ui->FPacketPeripheralFunction->setText(QString::number((int)fed.peripheral_function));
    ui->FPacketAction->setText(QString::number((int)fed.action));
    ui->FPacketData->setText(QString::number((float)fed.data));
    ui->FPacketFeedback->setText(QString::number((int)fed.time_stamp));


}

void MainWindow::log(QString msg,bool red){
    auto time = QDateTime::currentDateTime().toString("hh:mm:ss");
    if(red) ui->LogArea->setTextColor(QColor("red"));
    ui->LogArea->insertPlainText(time + " " + msg + "\n");

    ui->LogArea->setTextColor(QColor("white"));
}

void MainWindow::addPTZ(){
    PTZElement * ptz = new PTZElement(PTZs.size());
    ui->PTZsScrollArea->layout()->addWidget(ptz->getRoot());
    PTZs.push_back(ptz);
}

void MainWindow::autoMode()
{
    qDebug() << QString("Start Auto Mode");
    while(autoModeState.load() == On || true){
        if(this->connected == ConnectionStatus::On){
             retrevePTZsInformation();

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(this->refreshRateMS));

    }
    qDebug() << QString("NOT");

}

void MainWindow::setTargetAngle(int n,RotationDirection dir ,float angle)
{


    double upper , lower , delta;

    lower = modf(angle , &upper);
    lower *= 100;
    lower = (int) lower;

    requestSetTargetAngle(n,dir,(char)lower,false);

    if(upper > 127){
        requestSetTargetAngle(n,dir,(char)0,true);
        delta = (int) upper;
        // qDebug() << QString("delta sup %0 id %1").arg(delta).arg(n);
        while(delta > 127){
            requestAddTargetAngle(n,dir,(char)127,true);


            delta -= 127;
        }
       // qDebug() << QString("delta last %0 id %1").arg(delta).arg(n);

        requestAddTargetAngle(n,dir,(char)delta,true);
    }

    else requestSetTargetAngle(n,dir,(char)upper, true);




}


void MainWindow::initPTZs(){
    int n = requestNumberOfPTZs();
    float lat , lon;
    log("Number of PTZs in server : " + QString::number(n),false);
    for(int i = 0 ; i < n ; i++){
        addPTZ();
        lat = requestPTZLatitude(i+1);
        lon = requestPTZLongitude(i+1);
        PTZs[i]->setLat(lat);
        PTZs[i]->setLong(lon);
        PTZs[i]->setAlt(requestPTZAltitude(i+1));
        PTZs[i]->setRange(requestPTZRange(i+1));
        PTZs[i]->setPhi(requestPTZPhi(i+1));
        PTZs[i]->setref(requestPTZRef(i+1));
        PTZs[i]->setAzimuthAngle(requestPTZRotationAngle(n,RotationDirection::Azimuth));
        PTZs[i]->setElevationAngle(requestPTZRotationAngle(n,RotationDirection::Elevation));
        PTZs[i]->setFOV(requestPTZFOV(i+1));
        setTargetAngle(i+1,Elevation,25);
        setTargetAngle(i+1,Azimuth,100);
        mapMarker.addMarker(i,PTZs[i]);
        log("add PTZ .." , false);
    }
}

void MainWindow::initDrone()
{
    this->drone = new Drone(requestDroneLatitude(0),requestDroneLongitude(0),requestDroneAltitude(0));
    mapMarker.setDrone(this->drone);
    ui->PTZsScrollArea->layout()->addWidget(this->drone->getRoot());

}
void MainWindow::deletePTZs(){
    for(auto PTZ : PTZs){
        PTZ->getRoot()->deleteLater();
    }

    PTZs.clear();
    mapMarker.removeMarkerAll();

}
void MainWindow::retrevePTZsInformation(){
    int n;
    char r;
    PTZElement::Vector3d vec = {};
    for(int i = 0 ; i <  PTZs.size() ; i++){
        n = i+1;


        PTZs[i]->setAzimuthAngle(requestPTZRotationAngle(n,RotationDirection::Azimuth));
        PTZs[i]->setElevationAngle(requestPTZRotationAngle(n,RotationDirection::Elevation));
        vec = PTZs[i]->vectorToDrone(drone);

        if(!vec.isNull()){
            float azm = requestTargetAngle(n,Azimuth);
            float ele = requestTargetAngle(n,Elevation);
            if(abs(ele - vec.phiToTarget) > 0.01){
                setTargetAngle(n,Elevation,vec.phiToTarget);
                PTZs[i]->setTargetElevationAngle(ele);
            }

            if(abs(azm - vec.thetaToTarget) > 0.01){
                setTargetAngle(n,Azimuth,vec.thetaToTarget);
                PTZs[i]->setTargetAzimuthAngle(azm);

               // qDebug() << QString("setting %0 Target Azimuth : %1  Elevation %2 .").arg(n).arg(vec.thetaToTarget).arg(vec.phiToTarget);
               // qDebug() << QString("id %0 requset az %1 ele %2").arg(n).arg(requestTargetAngle(n,Azimuth)).arg(requestTargetAngle(n,Elevation));
            }



        }


        mapMarker.update(i,mapMarker.ViewLine);
        mapMarker.update(i,mapMarker.LeftLine);
        mapMarker.update(i,mapMarker.RightLine);
        mapMarker.update(i,mapMarker.DroneVector);
    }
    double lat = requestDroneLatitude(0);
    double lng = requestDroneLongitude(0);
    double alt = requestDroneAltitude(0);
    drone->setLatitude(lat);
    drone->setLongitude(lng);
    drone->setAltitude(alt);
  //  qDebug() << QString("Lat : %1 , Long : %2 , Alt : %3").arg(lat).arg(lng).arg(alt);

/*
    QGeoCoordinate final ;
    final = PTZs[3]->FindEndPoint(180,40);
    qDebug() << QString("180 Lat : %1 , Long : %2 , final lat : %3 final lng: %4").arg(PTZs[3]->getLat()).arg(PTZs[3]->getLong()).arg(final.latitude()).arg(final.longitude());

    final = PTZs[3]->FindEndPoint(0,40);
    qDebug() << QString("0 Lat : %1 , Long : %2 , final lat : %3 final lng : %4").arg(PTZs[3]->getLat()).arg(PTZs[3]->getLong()).arg(final.latitude()).arg(final.longitude());

    final = PTZs[3]->FindEndPoint(90,40);
    qDebug() << QString("90 Lat : %1 , Long : %2 , final lat : %3 final lng: %4").arg(PTZs[3]->getLat()).arg(PTZs[3]->getLong()).arg(final.latitude()).arg(final.longitude());

    final = PTZs[3]->FindEndPoint(45,40);
    qDebug() << QString("45 Lat : %1 , Long : %2 , final lat : %3 final lng : %4").arg(PTZs[3]->getLat()).arg(PTZs[3]->getLong()).arg(final.latitude()).arg(final.longitude());
*/
    mapMarker.updateDrone();

}


void MainWindow::updateRefreshRate()
{
    this->refreshRateMS = ui->RefreshRate->text().toInt();
}

void MainWindow::toggleAutoMode()
{
    this->autoModeState.store(this->autoModeState.load() == On ? Off : On);
}


void MainWindow::autoLoop(){


}


int MainWindow::requestNumberOfPTZs(){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = 0x0;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::NUMBER_OF_PTZ;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed))
        fed = sendPacket(&cmd);


    return (int)fed.data;


}
float MainWindow::requestPTZLongitude(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::LONGITUDE;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed))
        fed = sendPacket(&cmd);


    return fed.data;



}
float MainWindow::requestPTZFOV(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::FOV;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;



}

bool MainWindow::checkPacketValidaty(Packet::pktCommand *cmd, Packet::pktFeedback *fed)
{
    if(cmd->cmd == fed->cmd && cmd->peripheral == fed->peripheral && cmd->peripheral_function == fed->peripheral_function
            && cmd->action == fed->action)
            return true;

    qDebug() << QString("Corrupted Packet");
    return false;


}
float MainWindow::requestPTZLatitude(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::LATITUDE;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }

    return fed.data;
}

float MainWindow::requestSetTargetAngle(int n,RotationDirection dir, char angle , bool isUpper){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::WRITE;
    cmd.peripheral = n;
    cmd.peripheral_function = isUpper ? (dir == Elevation ? Packet::PeripheralFunction::SET_TARGET_ELEVATION_UPPER : Packet::PeripheralFunction::SET_TARGET_AZIMUTH_UPPER)
                                      : (dir == Elevation ? Packet::PeripheralFunction::SET_TARGET_ELEVATION_LOWER : Packet::PeripheralFunction::SET_TARGET_AZIMUTH_LOWER) ;
    cmd.action = angle;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }

    return fed.data;
}
float MainWindow::requestAddTargetAngle(int n,RotationDirection dir, char angle , bool isUpper){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::WRITE;
    cmd.peripheral = n;
    cmd.peripheral_function = isUpper ? (dir == Elevation ? Packet::PeripheralFunction::ADD_TARGET_ELEVATION_UPPER : Packet::PeripheralFunction::ADD_TARGET_AZIMUTH_UPPER)
                                      : (dir == Elevation ? Packet::PeripheralFunction::ADD_TARGET_ELEVATION_LOWER : Packet::PeripheralFunction::ADD_TARGET_AZIMUTH_UPPER) ;
    cmd.action = angle;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }

    return fed.data;
}

float MainWindow::requestTargetAngle(int n, MainWindow::RotationDirection dir)
{
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function = Packet::PeripheralFunction::INFORMATION;
    cmd.action = dir == Elevation ? Packet::Action::GET_TARGET_ELEVATION : Packet::Action::GET_TARGET_AZIMUTH;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }

    return fed.data;
}

float MainWindow::requestPTZAltitude(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::ALTITUDE;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;
}
float MainWindow::requestPTZRange(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::RANGE;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }

    return fed.data;
}
float MainWindow::requestPTZPhi(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::PHI;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }
    return fed.data;
}

float MainWindow::requestPTZRef(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::REF_FROM_NORTH;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;
}

float MainWindow::requestPTZRotationAngle(int n, RotationDirection dir){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = dir == RotationDirection::Azimuth ? Packet::Action::AZIMUTH : Packet::Action::ELEVATION;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;

}
float MainWindow::requestPTZMaxSpeed(int n, RotationDirection dir){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = dir == RotationDirection::Azimuth ? Packet::Action::AZIMUTH_SPEED : Packet::Action::ELEVATION_SPEED;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;
}
float MainWindow::requestPTZSpeedFactor(int n , RotationDirection dir){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = dir == RotationDirection::Azimuth ? Packet::Action::AZIMUTH_SPEED_FACTOR : Packet::Action::ELEVATION_SPEED_FACTOR;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;
}
float MainWindow::requestDroneLongitude(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::DRONE;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::LONGITUDE;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;

}
float MainWindow::requestDroneLatitude(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::DRONE;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::LATITUDE;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;

}
float MainWindow::requestDroneAltitude(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::DRONE;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::ALTITUDE;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;

}


MainWindow::~MainWindow()
{
    delete ui;
}

