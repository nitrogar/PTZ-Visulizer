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

private:
      int disconnectFromServer();
      int connectToServer();
      void log(QString msg,bool red);
      void autoLoop();
      void initPTZs();
      void deletePTZs();
      Packet::pktFeedback sendPacket(Packet::pktCommand * cmd);
      void addPTZ();
      void autoMode();
      int requestNumberOfPTZs();
      float requestPTZLongitude(int n);
      float requestPTZLatitude(int n);
      float requestPTZAltitude(int n);
      float requestPTZRange(int n);
      float requestPTZPhi(int n);
      float requestPTZRef(int n);

      float requestPTZRotationAngle(int n, RotationDirection dir);
      float requestPTZMaxSpeed(int n, RotationDirection dir);
      float requestPTZSpeedFactor(int n , RotationDirection dir);

};
#endif // MAINWINDOW_H
