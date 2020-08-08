#include "controlepoint2.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <cmath>
ControlePoint::ControlePoint(int id , QGeoCoordinate & center , QGeoCoordinate & pos):id(id),position(pos),center(center)
{
   this->color = QColor("red");
   setToolTip(QString("Point %0").arg(this->id));
   setCursor(Qt::OpenHandCursor);
   setAcceptedMouseButtons(Qt::LeftButton);


}

ControlePoint::ControlePoint()
{
    this->color = QColor("red");
    setToolTip(QString("Point %0").arg(this->id));
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    moveable = false;
}

QRectF ControlePoint::boundingRect() const
{
    return QRectF(-15.5, -15.5, 34, 34);
}

void ControlePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(color));
    painter->drawEllipse(-15, -15, 30, 30);
}

void ControlePoint::updateRelativePoint()
{
    float dist_x = center.distanceTo(QGeoCoordinate(center.latitude(),position.longitude()));
    float dist_y = center.distanceTo(QGeoCoordinate(position.latitude(), center.longitude()));
    float dist_z = position.altitude() - center.altitude();

    xy = QPointF(dist_x,dist_y);
    yz = QPointF(dist_y,dist_z);
}

void ControlePoint::updateRealPoint()
{
    float dist_x = xy.x();
    float dist_y = xy.y();
    //float dist_z = yz.y();
    float angle = atan2(dist_x,dist_y) * 180/M_PI;
    float bearing = angle > 0 ? angle : angle + 360;
    float dist = sqrt(dist_x*dist_x + dist_y *dist_y);

    position = center.atDistanceAndAzimuth(dist,bearing);
    qDebug() << position;
   //position.setAltitude(center.altitude()+dist_z);

}

void ControlePoint::setRelativePos(float x, float y, float z)
{
    this->xy.setX(x);
    this->xy.setY(y);
   // this->yz.setY(z);
    updateRealPoint();

}

void ControlePoint::setRealPos(QGeoCoordinate & pos)
{
    position = pos;
    updateRelativePoint();

}

void ControlePoint::setPos(const QPointF &pos)
{
   // qDebug() << QString("Custom setpos");


    QGraphicsItem::setPos(pos);

    if(this->view_xy){
        this->x = pos.x();
        this->y = pos.y();
    }

    else{
        this->y = pos.x();
        this->z = pos.y();
    }

    setRelativePos(pos.x(),pos.y(),0);

}


void ControlePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);

    moveable = true;
}

void ControlePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);
}

void ControlePoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(event->scenePos());
}

void ControlePoint::toggleViewPort()
{
    view_xy = ! view_xy;
    if(view_xy)
        setPos(QPointF(x,y));

    setPos(QPointF(y,z));

}

