#ifndef DRONEPATH_H
#define DRONEPATH_H

#include <QObject>
#include <QVector>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QPen>
#include "controlpoint.h"
class DronePath : public QObject
{
    Q_OBJECT
public:
    explicit DronePath(QObject *parent = nullptr);
    explicit DronePath(QGeoCoordinate center);
    void setCenter(QGeoCoordinate c);
    void addPoint(int id,QGeoCoordinate cp);
    QVector<ControlPoint *> & getControlPoints();
    QGraphicsPathItem & getPath();
    QGraphicsPathItem & getPointPath();

    QGeoCoordinate &current();

public slots:
    void updatePath();
signals:


private:

    QGeoCoordinate center;
    QVector<ControlPoint * > cps;
    QGraphicsPathItem path;
    QGraphicsPathItem connection;
    QPen pen1;
    QPen pen2;


};

#endif // DRONEPATH_H
