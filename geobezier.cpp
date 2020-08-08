#include "geobezier.h"
#include <cmath>
#include <QDebug>
GeoBezier::GeoBezier(QObject *parent)
{

}

void GeoBezier::setCenter(QGeoCoordinate c)
{
    center=c;
    point = c;
}

QGeoCoordinate &GeoBezier::getGeoPoint(int id)
{
    return p[id];

}

QGeoCoordinate GeoBezier::GeoAt(float t)
{
    QPointF p = Bezier::at(t);
    return localToGeo(p);
}

QGeoCoordinate GeoBezier::GeoAdvance(float t)
{
    QPointF p = Bezier::advance(t);
    return localToGeo(p);
}

void GeoBezier::updatePath(int id)
{
    Bezier::updatePath(id);
    updateGeoPoint(id);

}

void GeoBezier::updateGeoPoint(int id)
{
    QPointF po = {cps[id]->get_mx(),cps[id]->get_my()};
    p[id] = localToGeo(po);
   // qDebug() << position;
   //position.setAltitude(center.altitude()+dist_z);

}

QGeoCoordinate & GeoBezier::getCenter()
{
    return center;

}

QGeoCoordinate GeoBezier::localToGeo(QPointF & p)
{
    float dist_x = p.x();
    float dist_y = p.y();
    //float dist_z = z;
    float angle = atan2(dist_x,-1*dist_y) * 180/M_PI;
    float bearing = angle > 0 ? angle : angle + 360;
    float dist = sqrt(dist_x*dist_x + dist_y *dist_y);

    return center.atDistanceAndAzimuth(dist,bearing);

}

void GeoBezier::advanceGeoPoint(float t)
{
    advancePoint(t);
    QPointF x = {o.get_mx(),o.get_my()};
    point =  localToGeo(x);

}

QGeoCoordinate GeoBezier::getGeoM()
{
   // qDebug() << point;
    return point;

}
