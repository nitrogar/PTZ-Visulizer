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

}

void MainWindow::connectButtonStateMachine(){
    if(this->connected == ConnectionStatus::Off){
        if(connectToServer() < 0) return ;

        ui->connectPushButton->setText("Disconnect");
        this->connected = ConnectionStatus::On;
        initPTZs();
        this->autoModeState.store(On);
        this->autoRefreshThread = std::thread(&MainWindow::autoMode,this);

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
    while(autoModeState.load() == On){
        if(this->connected == ConnectionStatus::On){
             retrevePTZsInformation();

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(this->refreshRateMS));

    }

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
        mapMarker.addMarker(i,PTZs[i]);
        log("add PTZ .." , false);
    }





}
void MainWindow::deletePTZs(){
    for(auto PTZ : PTZs){
        PTZ->getRoot()->deleteLater();
    }

}
void MainWindow::retrevePTZsInformation(){
    int n;

    for(int i = 0 ; i <  PTZs.size() ; i++){
        n = i+1;

        PTZs[i]->setAzimuthAngle(requestPTZRotationAngle(n,RotationDirection::Azimuth));
        PTZs[i]->setElevationAngle(requestPTZRotationAngle(n,RotationDirection::Elevation));
        mapMarker.update(i,mapMarker.ViewLine);
        mapMarker.update(i,mapMarker.Azimuth);

    }
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
    return fed.data;



}
float MainWindow::requestPTZLatitude(int n){
    Packet::pktCommand cmd;
    Packet::pktFeedback fed;
    cmd.cmd = Packet::Command::READ;
    cmd.peripheral = n;
    cmd.peripheral_function =  Packet::PeripheralFunction::INFORMATION;
    cmd.action = Packet::Action::LATITUDE;

    fed = sendPacket(&cmd);
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
    return fed.data;
}

MainWindow::~MainWindow()
{
    delete ui;
}

