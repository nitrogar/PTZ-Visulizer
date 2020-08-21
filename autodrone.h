#ifndef AUTODRONE_H
#define AUTODRONE_H

#include <QObject>
#include <QGeoCoordinate>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QVector3D>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "bezier.h"
#include "geobezier.h"
class AutoDrone : public QObject
{
    Q_OBJECT

public:
    enum SceneType {XY , Z , SpeedCurve};
    explicit AutoDrone(QObject *parent = nullptr);
    QWidget * getInfoWidget();
    void initInfoWidget();
    QWidget * getGraphicWidget();
    void initGraphicWidget();
    void initScenes();
    void setPathGeoCenter(QGeoCoordinate c);
    GeoBezier * getlatLonCurve(){return & latLonCurve;};
    void advance(float t);
    void setScene(SceneType t);
    void setTime(float t);
    double getLatitude();
    double getLongitude();
    double getAltitude();
    QGeoCoordinate getPostition();
    void setSpeed(float s);
signals:
    void geoPosChanged(int n);
    void pointChanged(int);
public slots:
    void geoCPChanged(int n);
    void advanceDefult();
    void updateInfoWidget();

private:
    QGroupBox * root;
    QLineEdit *latEdit;
    QLineEdit *lngEdit;
    QLineEdit *altEdit;
    QVector3D velocity;
    GeoBezier latLonCurve;
    Bezier altCurve;
    Bezier speedCurve;
    QGraphicsScene * scene_xy;
    QGraphicsScene * scene_z;
    QGraphicsScene * scene_SpeedCurve;
    SceneType scene_type = XY;
    QGraphicsView * view;
    float unitToMeter = 1;
    float altTometer = 30;
    float circuitRaduis = 40;

};

#endif // AUTODRONE_H
