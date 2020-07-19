#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include "Packet.h"
#include "ptzelement.h"
#include "marker.h"
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots :
      void sendCustomePacket();
      void connectButtonStateMachine();
      void retrevePTZsInformation();
     // void addMarker();
      void updateRefreshRate();
      void toggleAutoMode();

signals :
      void connctionStatusChanged(int status);


private:

    Ui::MainWindow *ui;
    enum ConnectionStatus{Off, On, Wait};
    enum RotationDirection{Elevation,Azimuth};
    ConnectionStatus connected;
    std::atomic<ConnectionStatus> autoModeState;
    std::thread autoRefreshThread;
    int fdSocket;
    std::vector<PTZElement *> PTZs;
    int refreshRateMS;
    Marker mapMarker;
    Drone * drone;
private:
      int disconnectFromServer();
      int connectToServer();
      void log(QString msg,bool red);
      void autoLoop();
      void initPTZs();
      void initDrone();
      void deletePTZs();
      Packet::pktFeedback sendPacket(Packet::pktCommand * cmd);
      void addPTZ();
      void autoMode();
      void setTargetAngle(int n,RotationDirection dir ,float angle);
      int requestNumberOfPTZs();
      float requestPTZLongitude(int n);
      float requestPTZLatitude(int n);
      float requestPTZAltitude(int n);
      float requestPTZRange(int n);
      float requestPTZPhi(int n);
      float requestPTZRef(int n);
      float requestDroneLatitude(int n);
      float requestDroneLongitude(int n);
      float requestDroneAltitude(int n);
      float requestPTZRotationAngle(int n, RotationDirection dir);
      float requestPTZMaxSpeed(int n, RotationDirection dir);
      float requestPTZSpeedFactor(int n , RotationDirection dir);
      float requestPTZFOV(int n);
      bool checkPacketValidaty(Packet::pktCommand * cmd , Packet::pktFeedback * fed);
      float requestSetTargetAngle(int n,RotationDirection dir, char angle , bool isUpper);
      float requestAddTargetAngle(int n,RotationDirection dir, char angle , bool isUpper);
      float requestTargetAngle(int n,RotationDirection dir);


};
#endif // MAINWINDOW_H
