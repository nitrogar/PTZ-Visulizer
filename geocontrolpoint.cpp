#include "geocontrolpoint.h"
#include "controlpoint.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <cmath>
GeoControlPoint::GeoControlPoint(int id , QGeoCoordinate & center , QGeoCoordinate & pos):id(id),position(pos),center(center)
{
   this->color = QColor("red");
   setToolTip(QString("Point %0").arg(this->id));
   setCursor(Qt::OpenHandCursor);
   setAcceptedMouseButtons(Qt::LeftButton);


}

GeoControlPoint::GeoControlPoint()
{
    this->color = QColor("red");
    setToolTip(QString("Point %0").arg(this->id));
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    moveable = false;
}

QRectF GeoControlPoint::boundingRect() const
{
    return QRectF(-5.5,-5.5, 11, 11);
}


void GeoControlPoint::updateRelativePoint()
{
    float dist_x = center.distanceTo(QGeoCoordinate(center.latitude(),position.longitude()));
    float dist_y = center.distanceTo(QGeoCoordinate(position.latitude(), center.longitude()));
    float dist_z = position.altitude() - center.altitude();

    m_x = dist_x;
    m_y = dist_y;
    m_z = dist_z;
}

void GeoControlPoint::updateRealPoint()
{
    float dist_x = m_x;
    float dist_y = m_y;
    //float dist_z = z;
    float angle = atan2(dist_x,dist_y) * 180/M_PI;
    float bearing = angle > 0 ? angle : angle + 360;
    float dist = sqrt(dist_x*dist_x + dist_y *dist_y);

    position = center.atDistanceAndAzimuth(dist,bearing);
   // qDebug() << position;
   //position.setAltitude(center.altitude()+dist_z);

}

void GeoControlPoint::setRelativePos(float x, float y, float z)
{
    this->m_x = x;
    this->m_y = y;
    this->m_z = z;
    updateRealPoint();

}

void GeoControlPoint::setRealPos(QGeoCoordinate & pos)
{
    position = pos;
    updateRelativePoint();

}

void GeoControlPoint::setPos(const QPointF &pos)
{
   // qDebug() << QString("Custom setpos");


    QGraphicsItem::setPos(pos);

    if(this->view_xy){
        this->m_x = pos.x();
        this->m_y = pos.y();
    }

    else{
        this->m_y = pos.x();
        this->m_z = pos.y();
    }

    setRelativePos(pos.x(),pos.y(),0);

    emit posChanged();

}



QGeoCoordinate &GeoControlPoint::getGeoPostion()
{
    return position;

}

