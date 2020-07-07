#ifndef MARKER_H
#define MARKER_H

#include <QAbstractListModel>
#include <QList>
#include <QGeoCoordinate>
#include "ptzelement.h"
class Marker : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString current READ current)
    struct Tri{
        QGeoCoordinate v1;
        QGeoCoordinate v2;
        QGeoCoordinate v3;
    };
public:
    explicit Marker(QObject *parent = nullptr);
    enum{MarkerPostion = Qt::UserRole , Vertex1,Vertex2,Vertex3,Radius,CenterLine,CircleColor ,Number,Up,ViewLine,Azimuth};
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void update(int n ,int role);


    void addMarker(int row, PTZElement * p);
    void removeMarker(int row);
    QString current() const;
private:
    QList<PTZElement *> PTZ;

};

#endif // MARKER_H
