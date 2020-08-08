#ifndef GEOBEZIER_H
#define GEOBEZIER_H

#include "bezier.h"
class GeoBezier : public Bezier
{
    Q_OBJECT
public:
    explicit GeoBezier(QObject *parent = nullptr);
    explicit GeoBezier(QGeoCoordinate center);
    void setCenter(QGeoCoordinate c);
    QGeoCoordinate &getGeoPoint(int id);
    QGeoCoordinate  GeoAt(float t);
    QGeoCoordinate GeoAdvance(float t);
    void updateGeoPoint(int id);
    QGeoCoordinate &getCenter();
    QGeoCoordinate localToGeo(QPointF & p);
    void advanceGeoPoint(float t);
    QGeoCoordinate  getGeoM();
public slots:
    void updatePath(int id);

signals:


private:

    QGeoCoordinate center;
    QGeoCoordinate point;
    QGeoCoordinate p[4];


};

#endif // GEOBEZIER_H
