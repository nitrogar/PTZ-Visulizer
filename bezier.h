#ifndef BEZIER_H
#define BEZIER_H

#include <QObject>
#include "controlpoint.h"
#include <QPen>
class Bezier : public QObject
{
    Q_OBJECT
public:
    explicit Bezier(QObject *parent = nullptr);

    void addPoint(int id,QPointF p);
    QPointF  at(float t);
    QPointF advance(float t);
    QVector<ControlPoint *> & getControlPoints();
    QGraphicsPathItem & getPath();
    QGraphicsPathItem & getPointPath();
    ControlPoint & getM(){return o;};
    void advancePoint(float t);
    void setTime(float t);
    double getLength();

public slots:
    void updatePath(int id);
signals:
    void posChanged(int id);

protected:

    ControlPoint o;
    QVector<ControlPoint * > cps;
    QGraphicsPathItem path;
    QGraphicsPathItem connection;
    QPen pen1;
    QPen pen2;
    float t_total;

};

#endif // BEZIER_H
