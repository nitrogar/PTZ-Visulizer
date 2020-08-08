#ifndef GEOCONTROLPOINT_H
#define GEOCONTROLPOINT_H
#include "controlpoint.h"

class GeoControlPoint : public ControlPoint
{
    Q_OBJECT

public:
     GeoControlPoint(int id ,QGeoCoordinate &center ,QGeoCoordinate &pos);
     GeoControlPoint();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void updateRelativePoint();
    void updateRealPoint();
    void setRelativePos(float x , float y , float z);
    void setRealPos(QGeoCoordinate  & pos);
    void setPos(const QPointF &pos);
    void toggleViewPort();
    QGeoCoordinate & getGeoPostion();
signals:
    void posChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int id;
    float m_x;
    float m_y;
    float m_z;
    QGeoCoordinate position;
    QGeoCoordinate center;
    QColor color;
    bool moveable;
    bool view_xy;
};


#endif // GEOCONTROLPOINT_H
