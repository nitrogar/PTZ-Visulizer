#include "controlpoint.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <cmath>
ControlPoint::ControlPoint(int id,float x , float y):id(id),m_x(x),m_y(y)
{
   this->color = QColor("red");
   setToolTip(QString("Point %0").arg(this->id));
   setCursor(Qt::OpenHandCursor);
   setAcceptedMouseButtons(Qt::LeftButton);


}

ControlPoint::ControlPoint(int id, QPointF &p):ControlPoint(id,p.x(),p.y())
{

}

ControlPoint::ControlPoint():ControlPoint(0,0,0)
{

}


QRectF ControlPoint::boundingRect() const
{
    return QRectF(-5.5,-5.5, 11, 11);
}

void ControlPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(color));
    painter->drawEllipse(-5, -5, 10, 10);
}


void ControlPoint::setPos(const QPointF &pos)
{
   // qDebug() << QString("Custom setpos");


    QGraphicsItem::setPos(pos);

    this->m_x = pos.x();
    this->m_y = pos.y();
    emit posChanged(id);

}

float ControlPoint::get_mx()
{
    return m_x;

}

float ControlPoint::get_my()
{
    return m_y;

}


void ControlPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);

    moveable = true;
}

void ControlPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);
}

void ControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(event->scenePos());
}

QPointF operator *(float f, ControlPoint &v)
{
        return QPointF( v.get_mx() * f, v.get_my() * f);
}
