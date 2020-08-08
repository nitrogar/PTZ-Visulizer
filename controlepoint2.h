#ifndef CONTROLEPOINT_H
#define CONTROLEPOINT_H

#include <QGraphicsItem>
#include <QGeoCoordinate>
class ControlePoint : public QGraphicsItem
{
public:
    ControlePoint(int id ,QGeoCoordinate &center ,QGeoCoordinate &pos);
    ControlePoint();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void updateRelativePoint();
    void updateRealPoint();
    void setRelativePos(float x , float y , float z);
    void setRealPos(QGeoCoordinate  & pos);
    void setPos(const QPointF &pos);
    void toggleViewPort();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int id;
    float x;
    float y;
    float z;
    QGeoCoordinate position;
    QGeoCoordinate center;
    QColor color;
    bool moveable;
    bool view_xy;

};

#endif // CONTROLEPOINT_H
