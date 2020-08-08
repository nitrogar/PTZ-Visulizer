#ifndef DRONE_H
#define DRONE_H

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QGeoCoordinate>
#include "dronepath.h"
class Drone {

public:
    struct location{
        double latitude;
        double longitude;
        double altitude;
    };

    struct Vector{
        double x_vec;
        double y_vec;
        double z_vec;
    };
    double getLatitude() const;
    void setLatitude(double latitude);
    double getLongitude() const;
    void setLongitude(double longitude);
    double getAltitude() const;
    void setAltitude(double altitude);
    double getXVec() const;
    void setXVec(double xVec);
    double getYVec() const;
    void setYVec(double yVec);
    double getZVec() const;
    void setZVec(double zVec);
    QGeoCoordinate getPosition();
    Drone(double latitude, double longitude, double altitude);
    Drone(double latitude, double longitude, double altitude, double xVec, double yVec, double zVec);
    Drone();
    QWidget *getRoot();


private:
    QGroupBox * root;
    QLineEdit *latEdit;
    QLineEdit *lngEdit;
    QLineEdit *altEdit;
    QGeoCoordinate current;
    Vector velocity;
    //DronePath path;
    /**
    mainWin.cpp remove droen code with simulator
    add drone.addCp()
    add new model for cp
    add curve function
    check conncetivity


*/


};


#endif // DRONE_H
