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
class Drone;
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
    Vector3d vectorToDrone(Drone * d);
    QGeoCoordinate FindEndPointWithRefrece(double angle,double r);
    QGeoCoordinate caclculatedEndPoint(Drone * drone);
    QGeoCoordinate caclculatedEndPoint(QGeoCoordinate drone);
    QGeoCoordinate getPostion();
    QGeoCoordinate getEndLine();
    QGroupBox * getRoot();
    QGeoCoordinate getViewLine();
    QGeoCoordinate FindEndPoint(double angle,double r);

    Vector3d getVectorTo(double lat,double lng,double alt);
    float haversine(double lat0, double lng0,double lat1, double lng1);
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
    QString circleColor;
    Vector3d toTarget;

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
