#include "dronepath.h"
#include <QDebug>
DronePath::DronePath(QObject *parent) : QObject(parent)
{

   pen1.setCapStyle(Qt::PenCapStyle::RoundCap);
   pen1.setStyle(Qt::PenStyle::DashDotDotLine);
   pen1.setWidth(5);

   pen2.setCapStyle(Qt::PenCapStyle::FlatCap);
   pen2.setStyle(Qt::PenStyle::SolidLine);
   pen2.setWidth(2);
   pen2.setColor(QColor("gold"));
}

void DronePath::setCenter(QGeoCoordinate c)
{
    center = c;

}

void DronePath::addPoint(int id, QGeoCoordinate cp)
{
    if(id > 3) return;

  //  ControlPoint * p = new ControlPoint(id , center,cp);
    //cps.insert(id,p);
   // connect(p,SIGNAL(posChanged()),this,SLOT(updatePath()));
}

QVector<ControlPoint *> &DronePath::getControlPoints()
{
    return cps;
}

QGraphicsPathItem &DronePath::getPath()
{
    return path;
}

QGraphicsPathItem &DronePath::getPointPath()
{
    return connection;
}

QGeoCoordinate &DronePath::current()
{
    return ->getGeoPostion();
}

void DronePath::updatePath()
{
  //  qDebug() << QString("Changed") ;
    if(cps.count() < 4) return;


    QPainterPath p(cps[0]->pos());
    p.cubicTo(cps[1]->pos() , cps[2]->pos() , cps[3]->pos());
    path.setPen(pen1);
    path.setPath(p);
    QPainterPath s(cps[0]->pos());
    for(int i = 1 ; i < cps.count() ; i++){
        s.lineTo(cps[i]->pos());
    }

   connection.setPath(s);
   connection.setPen(pen2);


}
