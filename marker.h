#ifndef MARKER_H
#define MARKER_H

#include <QAbstractListModel>
#include <QList>
#include <QGeoCoordinate>
#include "ptzelement.h"
#include "drone.h"
#include "autodrone.h"
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
    enum{MarkerPostion = Qt::UserRole , Vertex1,Vertex2,Vertex3,Radius,CenterLine,CircleColor ,Number,Up,ViewLine,Azimuth,Elevation,
           Theta,R, Phi,Ro,CaclulatedLine,DronePostion,DroneVector,LeftLine,RightLine , HalfRightLine,HalfLeftLine,CP1,CP2,CP3,CP4,CP5
        };
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
    void setDrone(Drone * d);
    void updateDrone();
    void removeMarkerAll();
    void setAutoDrone(AutoDrone * p){ad = p;};
public slots:
    void updateCP(int n);

private:
    QList<PTZElement *> PTZ;
    Drone * drone;
    AutoDrone * ad;
};

#endif // MARKER_H
