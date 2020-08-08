#ifndef ControlPoint_H
#define ControlPoint_H
#include <QGraphicsObject>
#include <QGeoCoordinate>
class ControlPoint : public QGraphicsObject
{
    Q_OBJECT
public:
    ControlPoint(int id ,float x, float y);
    ControlPoint(int id,QPointF & p);
    ControlPoint();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setPos(const QPointF &pos);
    QPointF & operator *(float scale);
    float get_mx();
    float get_my();
signals:
    void posChanged(int id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int id;
    float m_x;
    float m_y;
    QColor color;
    bool moveable;
    bool view_xy;

};
QPointF operator * ( float f, ControlPoint& v );


#endif // ControlPoint_H
