#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include "Packet.h"
#include "ptzelement.h"
#include "marker.h"
#include <vector>
#include "autodrone.h"
#include <QTimer>
#include <QtCharts>
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
    void listenController();
    void updatePTZCo();
     // void addMarker();
    void updateRefreshRate();
    void toggleAutoMode();
    //  void updateCP(int n);
    void setSceneXY();
    void setSceneZ();
    void setSceneSpeed();
    void startCurve();
    void stopCurve();
    void resetCurve();
    void autoMode();
    void prevPTZ();
    void nextPTZ();
    // for testing
    void updatePTZTrackSpeed();
    void updateDroneSpeed();
    // end for testing

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
    int currentPTZ = 0;
    Marker mapMarker;
    Drone * drone;
    AutoDrone * a_drone;
    QTimer tick;
    QGraphicsView view;
    QtCharts::QChartView chartView;
    QtCharts::QChartView chartSpeedView;
    std::thread droneInputThread;
    int PTZCo = 0;
private:
      int disconnectFromServer();
      int connectToServer();
      void log(QString msg,bool red);
      void autoLoop();
      void initPTZs();
      void initDrone();
      void deletePTZs();
      void droneInput();
      Packet::pktFeedback sendPacket(Packet::pktCommand * cmd);
      void addPTZ();
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
      float requestPTZSpeed(int n, RotationDirection dir);
      float requestPTZMaxSpeed(int n, RotationDirection dir);
      float requestPTZSpeedFactor(int n , RotationDirection dir);
      float requestPTZFOV(int n);
      bool checkPacketValidaty(Packet::pktCommand * cmd , Packet::pktFeedback * fed);
      float requestSetTargetAngle(int n,RotationDirection dir, char angle , bool isUpper);
      float requestAddTargetAngle(int n,RotationDirection dir, char angle , bool isUpper);
      float requestTargetAngle(int n,RotationDirection dir);
      float requestLoadTargetAngle(int n,RotationDirection dir);
      float requestSetSpeedFactor(int n,char k,RotationDirection dir);
      float requestSetSpeed(int n,RotationDirection dir, char sp , bool isUpper);
      float requestLoadSpeed(int n , RotationDirection dir);
      void setPTZSpeed(int n,RotationDirection dir ,float angle);

      void setPTZScene(QGraphicsScene &sc);
      void setPTZChart(QtCharts::QChart & ch ,QChart &speedch);


};
#endif // MAINWINDOW_H
