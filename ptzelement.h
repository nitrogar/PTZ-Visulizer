#ifndef PTZELEMENT_H
#define PTZELEMENT_H
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QGeoCoordinate>
#include <cmath>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QValueAxis>
class Drone;
class AutoDrone;
class PTZElement
{

public:
    struct  Vector3d{
        double thetaToTarget;
        double phiToTarget;
        double roToTarget;
        double r;
        bool isNull(){
            return thetaToTarget == 0 && phiToTarget == 0 && roToTarget == 0;
        }
    };

   struct Factors{
    float Azm;
    float Ele;
    bool isNull(){
        return Azm == 0 && Ele;
    }

   };


    PTZElement(int id);
    void setLat(float lat);
    void setLong(float lng);
    void setAlt(float lat);
    void setRange(float lng);
    void setref(float lat);
    void setFOV(float fov);
    void setPhi(float lng);
    void setAzimuthAngle(float angle);
    void setElevationAngle(float angle);
    void setTargetAzimuthAngle(float angle);
    void setTargetElevationAngle(float angle);
    float getLat();
    float getLong();
    float getElevationAngle();
    float getAzimuthAngle();
    QGeoCoordinate LeftViewPoint();
    QGeoCoordinate RightViewPoint();
    QGeoCoordinate HalfLeftViewPoint();
    QGeoCoordinate HalfRightViewPoint();
    Vector3d vectorToDrone(AutoDrone * d);
    QGeoCoordinate FindEndPointWithRefrece(double angle,double r);
    QGeoCoordinate caclculatedEndPoint(AutoDrone * drone);
    QGeoCoordinate caclculatedEndPoint(QGeoCoordinate drone);
    QGeoCoordinate getPostion();
    QGeoCoordinate getEndLine();
    QGroupBox * getRoot();
    QGeoCoordinate getViewLine();
    QGeoCoordinate FindEndPoint(double angle,double r);

    Vector3d getVectorTo(double lat,double lng,double alt);
    float haversine(double lat0, double lng0,double lat1, double lng1);

    QGeoCoordinate positionAt(AutoDrone * d ,float t);
    QVector3D positionXYZAt(AutoDrone * d ,float t);
    void addToHistory(QGeoCoordinate & g);
    QGeoCoordinate cartToGeo(QVector3D & v);
    QVector3D  GeoTocart(QGeoCoordinate & g);
    QVector3D  velocity(float t);
    Factors  caclulateFastes(AutoDrone *d,float dist,float zdist);
    Factors  droneScan(AutoDrone * d,float timeElapsed);
    void updateVelocityParameters(float elapsed,float dist);
    float motionFunction(float dd);
    float  calculateBearing(float x , float y);
    float  calculateTheta(float x , float y);
    float error(float a, float b);
    float calculateOrbitalVelocity(int isBearing ,QPointF a,QPointF b);
    double cubeRoot(double x);
    bool approx(float a ,float b);
    double * sloveCubic(float a,float b,float c,float d,float e);
    float calculateDistance(QGeoCoordinate & a, QGeoCoordinate & b);



    void initScene();
    void initChart();
    void addPredictPoint(QPointF & p);
    void addRealPoint(QPointF & p);
    void clearPrePath();


    int  id;
    double latitude;
    double longitude;
    double altitude;
    double range;
    double fov;
    double angleFromNorth;
    double Phi;
    double azimuth;
    double elevation;
    double azimuthAnglurSpeed;
    double elevationAnglurSpeed;
    double maxAzimuthAnglurSpeed;
    double maxElevationAnglurSpeed;
    float oldAzimuth = 0;
    float oldElevation = 0;
    QString circleColor;
    Vector3d toTarget;
    QVector<QVector3D> historyXYZ;
    QVector<QGeoCoordinate> historyGeo;
    QVector<float> param {0,0,0};
    QGraphicsScene scene;
    QPainterPath painterPath;
    QPainterPath dronePainterPath;
    QGraphicsPathItem dronePath;
    QGraphicsPathItem predictedPath;
    QtCharts::QChart rmsChart;
    QtCharts::QChart rmsSpeedChart;

    float total_time = 0;
    QGeoCoordinate droneEndpoint;
    QtCharts::QLineSeries errAzimuthSerise;
    QtCharts::QLineSeries errElevationSerise;
    QtCharts::QLineSeries errSpeedSerise;
    QtCharts::QLineSeries errElevationSpeedSerise;

    QtCharts::QValueAxis AX;
    QtCharts::QValueAxis AY;
    QtCharts::QValueAxis AX2;
    QtCharts::QValueAxis AY2;
    double prevAzm = 0,prevEle = 0 , dAzm = 0, dEle = 0;
    bool isfirstPointInPredection = true;
private:

       //const double DEG_TO_RAD = 0.017453292519943295769236907684886;
       const double DEG_TO_RAD = M_PI/180.0f;

       //const double EARTH_RADIUS_IN_METERS = 6372797.560856;
       const double EARTH_RADIUS_IN_METERS = 6378137;

       QGroupBox *root;
       QVBoxLayout *verticalLayout_9;
       QHBoxLayout *horizontalLayout;
       QLabel *PTZlabelLat;
       QLineEdit *PTZLat;
       QHBoxLayout *horizontalLayout_18;
       QLabel *PTZlabelLong;
       QLineEdit *PTZLong;
       QHBoxLayout *horizontalLayout_19;
       QLabel *PTZlabelAz;
       QLineEdit *PTZAzimuth;
       QHBoxLayout *horizontalLayout_9;
       QLabel *PTZlabelEl;
       QLineEdit *PTZElevation;
       QHBoxLayout *horizontalLayout_10;
       QLabel *PTZlabelAlt;
       QLineEdit *PTZAltitude;

       QHBoxLayout *horizontalLayout_11;
       QLabel *PTZlabelRange;
       QLineEdit *PTZRange;


       QHBoxLayout *horizontalLayout_13;
       QLabel *PTZlabelPhi;
       QLineEdit *PTZPhi;


       QHBoxLayout *horizontalLayout_14;
       QLabel *PTZlabelRef;
       QLineEdit *PTZRef;

       QHBoxLayout *horizontalLayout_12;
       QLabel *PTZlabelcenterLine;
       QLineEdit *PTZCenterLine;


       QHBoxLayout *horizontalLayout_15;
       QLabel *PTZlabelTargetAzimuth;
       QLineEdit *PTZTargetAzimuth;


       QHBoxLayout *horizontalLayout_16;
       QLabel *PTZlabelTargetElevation;
       QLineEdit *PTZTargetElevation;
};

#endif // PTZELEMENT_H
