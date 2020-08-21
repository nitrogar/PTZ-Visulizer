#include "bezier.h"
#include <cmath>
#include <QDebug>
Bezier::Bezier(QObject *parent) : QObject(parent)
{
    t_total = 0;
    d_total = 0;
    length = 0;
    speed = 0;
    pen1.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen1.setStyle(Qt::PenStyle::DashDotDotLine);
    pen1.setWidth(5);

    pen2.setCapStyle(Qt::PenCapStyle::FlatCap);
    pen2.setStyle(Qt::PenStyle::SolidLine);
    pen2.setWidth(2);
    pen2.setColor(QColor("gold"));
}

QPointF Bezier::at(float t)
{
    return (1-t)*(1-t)*(1-t)*(*cps[0]) + 3*(1-t)*(1-t)*t*(*cps[1]) + 3*(1-t)*t*t*(*cps[2]) + t*t*t*(*cps[3]);

}

QPointF Bezier::advance(float d)
{
    d_total += d;
    if(d_total > 1) d_total = 1;
    return at(d_total);

}



void Bezier::addPoint(int id, QPointF p)
{
    if(id > 3) return;

    ControlPoint * cp = new ControlPoint(id , p);
    cps.insert(id,cp);
    connect(cp,SIGNAL(posChanged(int)),this,SLOT(updatePath(int)));
}

QVector<ControlPoint *> &Bezier::getControlPoints()
{
    return cps;
}

QGraphicsPathItem &Bezier::getPath()
{
    return path;
}

QGraphicsPathItem &Bezier::getPointPath()
{
    return connection;
}

void Bezier::advancePoint(float t)
{
    if(d_total >= 1) return;
    float d = t * speed/length;

    o.setPos(advance(d));
    emit posChanged(4);

}

void Bezier::setTime(float t)
{
    d_total = t;


}

double Bezier::getLength()
{
    double r = 0;
    for(int i = 0 ; i < 3 ; i++)
        r += sqrt((cps[i]->get_mx() - cps[i+1]->get_mx())*(cps[i]->get_mx() - cps[i+1]->get_mx())+(cps[i]->get_my() - cps[i+1]->get_my())*(cps[i]->get_my() - cps[i+1]->get_my()));

    return r;

}

float Bezier::getL(){

    return length;
}

void Bezier::setSpeed(float s)
{
    speed = s;
}

void Bezier::updatePath(int id)
{
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
   length = getLength();
   emit posChanged(id);


}
