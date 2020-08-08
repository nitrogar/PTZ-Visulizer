#include "bezier.h"

Bezier::Bezier(QObject *parent) : QObject(parent)
{
    t_total = 0;
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

QPointF Bezier::advance(float t)
{
    t_total += t;
    if(t_total > 1) t_total = 1;
    return at(t_total);

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
    if(t_total >= 1) return;
    o.setPos(advance(t));
    emit posChanged(4 );

}

void Bezier::setTime(float t)
{
    t_total = t;

}

double Bezier::getLength()
{



}



void Bezier::updatePath(int id)
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

   emit posChanged(id);


}
