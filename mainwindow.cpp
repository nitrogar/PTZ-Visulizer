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
#include <QGraphicsScene>
#include "controlepoint2.h"
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
    connect(ui->XY,SIGNAL(clicked(bool)),this,SLOT(setSceneXY()));
    connect(ui->Z,SIGNAL(clicked(bool)),this,SLOT(setSceneZ()));
    connect(ui->Speed,SIGNAL(clicked(bool)),this,SLOT(setSceneSpeed()));
    connect(ui->startCurve,SIGNAL(clicked(bool)),this,SLOT(startCurve()));
    connect(ui->stopCurve,SIGNAL(clicked(bool)),this,SLOT(stopCurve()));
    connect(ui->resetCurve,SIGNAL(clicked(bool)),this,SLOT(resetCurve()));

    connect(ui->connectPushButton,SIGNAL(clicked(bool)),this,SLOT(connectButtonStateMachine()));
    connect(ui->updatePTZs,SIGNAL(clicked(bool)),this ,SLOT(retrevePTZsInformation()));
   // connect(ui->addMarker, SIGNAL(clicked(bool)), this , SLOT(addMarker()));
    connect(ui->RefreshRate,SIGNAL(editingFinished()),this,SLOT(updateRefreshRate()));
    connect(ui->autoModeToggle,SIGNAL(clicked(bool)),this ,SLOT(toggleAutoMode()));
/*
    QGraphicsScene * scene = new QGraphicsScene();
    QGeoCoordinate pos = QGeoCoordinate(24.691192, 46.699733);
    ControlePoint *cp = new ControlePoint(1,pos,pos);
    scene->addItem(cp);
    ui->latLonGraphicsView->setScene(scene);

*/
    this->refreshRateMS = 100;
    this->a_drone = new AutoDrone();
    a_drone->setPathGeoCenter(QGeoCoordinate(24.684353, 46.695205));
    mapMarker.setAutoDrone(a_drone);
    QVBoxLayout * vb = new QVBoxLayout();
    vb->addWidget(a_drone->getGraphicWidget());
    ui->DroneWidget->setLayout(vb);
    ui->PTZsScrollArea->layout()->addWidget(a_drone->getInfoWidget());


    QVBoxLayout * vb2 = new QVBoxLayout();
    ui->PTZChart->setLayout(vb2);
    ui->PTZChart->layout()->addWidget(&view);
    ui->PTZChart->layout()->addWidget(&chartView);
    connect(a_drone,SIGNAL(geoPosChanged(int)),&mapMarker,SLOT(updateCP(int)));
    connect(&tick, SIGNAL(timeout()), this, SLOT(autoMode()));
    connect(ui->PrevPTZ,SIGNAL(clicked(bool)),this ,SLOT(prevPTZ()));
    connect(ui->NextPTZ,SIGNAL(clicked(bool)),this ,SLOT(nextPTZ()));

// add PTZChart , do the button connect , do slot to switch ptz , move chartView and grahics view from the ptz to mainwin to just setScence , set Chart
    // PLUS minus movment dir not im


}

void MainWindow::connectButtonStateMachine(){
    if(this->connected == ConnectionStatus::Off){
        if(connectToServer() < 0) return ;

        ui->connectPushButton->setText("Disconnect");
        this->connected = ConnectionStatus::On;
        initDrone();
        initPTZs();


        tick.start(refreshRateMS);
        //this->autoRefreshThread = std::thread(&MainWindow::autoMode,this);
    }
    else {
        if(disconnectFromServer() < 0) return ;
        this->connected = ConnectionStatus::Off;
        ui->connectPushButton->setText("Connect");
        deletePTZs();
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
    if(this->connected == ConnectionStatus::On){
             retrevePTZsInformation();
             a_drone->advanceDefult();
    }

}

void MainWindow::prevPTZ()
{
   if(PTZs.empty()) return;
   currentPTZ--;

  if(currentPTZ > PTZs.size()) currentPTZ = PTZs.size() - 1;
  if(currentPTZ < 0) currentPTZ = 0;

   setPTZScene(PTZs[currentPTZ]->scene);
   setPTZChart(PTZs[currentPTZ]->rmsChart);

   ui->PTZNumber->setText(QString::number(currentPTZ));



}

void MainWindow::nextPTZ()
{
    if(PTZs.empty()) return;
    currentPTZ++;

   if(currentPTZ >= PTZs.size()) currentPTZ = PTZs.size() - 1;
   if(currentPTZ < 0) currentPTZ = 0;

    setPTZScene(PTZs[currentPTZ]->scene);
    setPTZChart(PTZs[currentPTZ]->rmsChart);
    ui->PTZNumber->setText(QString::number(currentPTZ));


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
        PTZs[i]->azimuthAnglurSpeed = requestPTZMaxSpeed(i+1,Azimuth);
        PTZs[i]->elevationAnglurSpeed = requestPTZMaxSpeed(i+1,Elevation);
        setTargetAngle(i+1,Elevation,25);
        setTargetAngle(i+1,Azimuth,100);
        mapMarker.addMarker(i,PTZs[i]);
        log("add PTZ .." , false);
    }
}

void MainWindow::initDrone()
{




    //Auto one

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
    PTZElement::Factors f = {};
    for(int i = 0 ; i <  PTZs.size() ; i++){
        n = i+1;


        PTZs[i]->setAzimuthAngle(requestPTZRotationAngle(n,RotationDirection::Azimuth));
        PTZs[i]->setElevationAngle(requestPTZRotationAngle(n,RotationDirection::Elevation));
        f = PTZs[i]->droneScan(a_drone,tick.interval());

        setPTZSpeed(n,Azimuth,f.Azm);
        setPTZSpeed(n,Elevation,f.Ele);

        /*
        if(!f.isNull()){
            float azm = requestTargetAngle(n,Azimuth);
            float ele = requestTargetAngle(n,Elevation);
            if(abs(ele - f.Ele) > 0.01){
               // setTargetAngle(n,Elevation,f.phiToTarget);
                requestSetSpeedFactor(n,f.Ele,Elevation);
                PTZs[i]->setTargetElevationAngle(ele);
            }

            if(abs(azm - f.Azm) > 0.01){

               // setTargetAngle(n,Azimuth,vec.thetaToTarget);
                PTZs[i]->setTargetAzimuthAngle(azm);
                requestSetSpeedFactor(n,f.Azm,Azimuth);

               // qDebug() << QString("setting %0 Target Azimuth : %1  current: %2  .").arg(n).arg(f.Azm).arg(azm);
                // qDebug() << QString("id %0 requset az %1 ele %2").arg(n).arg(requestTargetAngle(n,Azimuth)).arg(requestTargetAngle(n,Elevation));
            }

       }
        */




        mapMarker.update(i,mapMarker.ViewLine);
        mapMarker.update(i,mapMarker.LeftLine);
        mapMarker.update(i,mapMarker.RightLine);
        mapMarker.update(i,mapMarker.DroneVector);
    }
    /*
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
    mapMarker.updateDrone();
*/

}


void MainWindow::updateRefreshRate()
{
    this->refreshRateMS = ui->RefreshRate->text().toInt();
}

void MainWindow::toggleAutoMode()
{
    this->autoModeState.store(this->autoModeState.load() == On ? Off : On);
}

void MainWindow::setSceneXY()
{
    a_drone->setScene(AutoDrone::XY);
}

void MainWindow::setSceneZ()
{
    a_drone->setScene(AutoDrone::Z);
}

void MainWindow::setSceneSpeed()
{
    a_drone->setScene(AutoDrone::SpeedCurve);
}

void MainWindow::startCurve()
{
    tick.start(100);
}

void MainWindow::stopCurve()
{
    tick.stop();
}

void MainWindow::resetCurve()
{
    a_drone->setTime(0);
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

float MainWindow::requestSetSpeed(int n,RotationDirection dir, char sp , bool isUpper){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::WRITE;
    cmd.peripheral = n;
    cmd.peripheral_function = isUpper ? (dir == Elevation ? Packet::PeripheralFunction::SET_ELEVATION_SPEED_UPPER : Packet::PeripheralFunction::SET_AZIMUTH_SPEED_UPPER)
                                      : (dir == Elevation ? Packet::PeripheralFunction::SET_ELEVATION_SPEED_LOWER : Packet::PeripheralFunction::SET_AZIMUTH_SPEED_LOWER) ;
    cmd.action = sp;

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

float MainWindow::requestLoadTargetAngle(int n, MainWindow::RotationDirection dir)
{
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::WRITE;
    cmd.peripheral = n;
    cmd.peripheral_function = dir == Azimuth ? Packet::PeripheralFunction::LOAD_TARGET_AZIMUTH : Packet::PeripheralFunction::LOAD_TARGET_ELEVATION;
    cmd.action = 0;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }


    return fed.data;

}

float MainWindow::requestSetSpeedFactor(int n, char k, MainWindow::RotationDirection dir)
{

    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::WRITE;
    cmd.peripheral = n;
    cmd.peripheral_function =  dir == Azimuth ? Packet::PeripheralFunction::SET_AZIMUTH_SPEED_FACTOR : Packet::PeripheralFunction::SET_ELEVATION_SPEED_FACTOR ;
    cmd.action = k;

    fed = sendPacket(&cmd);
    while(!checkPacketValidaty(&cmd,&fed)){
        qDebug() << QString("Resend");
        fed = sendPacket(&cmd);
    }

    return fed.data;
}

void MainWindow::setPTZScene(QGraphicsScene &sc)
{
    view.setScene(&sc);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::setPTZChart(QChart &ch)
{
    chartView.setChart(&ch);
    chartView.setRenderHint(QPainter::Antialiasing);
    chartView.setRubberBand(QChartView::RectangleRubberBand);
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
void MainWindow::setPTZSpeed(int n,RotationDirection dir ,float angle)
{


    double upper , lower;

    lower = modf(angle , &upper);
    lower *= 100;
    lower = (int) lower;

    requestSetSpeed(n,dir,(char)lower,false);
    requestSetSpeed(n,dir,(char)upper, true);
    requestLoadSpeed(n,dir);




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


    if(dir == Azimuth)
        requestLoadTargetAngle(n,Azimuth);

    else
        requestLoadTargetAngle(n,Elevation);


}
float MainWindow::requestLoadSpeed(int n , RotationDirection dir){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::WRITE;
    cmd.peripheral = n;
    cmd.peripheral_function =  dir == RotationDirection::Azimuth ? Packet::PeripheralFunction::LOAD_SPEED_AZIMUTH : Packet::PeripheralFunction::LOAD_SPEED_ELEVATION;
    cmd.action = 0;
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

