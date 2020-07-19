#include "marker.h"
#include <QDebug>
Marker::Marker(QObject *parent)
    : QAbstractListModel(parent)
{
}

int Marker::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return PTZ.count();
}

QVariant Marker::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    if(role == MarkerPostion)
        return QVariant::fromValue(PTZ[index.row()]->getPostion());
    //if(role == Vertex1)
        //   return QVariant::fromValue(QString("TESSSET"));

    else if(role == Radius)
        return QVariant::fromValue(PTZ[index.row()]->range);


    else if(role == Number)
            return QVariant::fromValue(PTZ[index.row()]->id);
   else if(role == CircleColor)
        return QVariant::fromValue(PTZ[index.row()]->circleColor);

    else if(role == CenterLine)
        return QVariant::fromValue(PTZ[index.row()]->getEndLine());

    else if(role == Up)
        return QVariant::fromValue(QGeoCoordinate(PTZ[index.row()]->latitude + 1,PTZ[index.row()]->longitude));

    else if(role == ViewLine)
        return QVariant::fromValue(PTZ[index.row()]->getViewLine());

    else if(role == Azimuth)
        return QVariant::fromValue(PTZ[index.row()]->getAzimuthAngle());

    else if(role == Phi)
        return QVariant::fromValue(PTZ[index.row()]->vectorToDrone(drone).phiToTarget);

    else if(role == Elevation)
        return QVariant::fromValue(PTZ[index.row()]->getElevationAngle());

    else if(role == Ro)
        return QVariant::fromValue(PTZ[index.row()]->vectorToDrone(drone).roToTarget);

    else if(role == Theta)
        return QVariant::fromValue(PTZ[index.row()]->vectorToDrone(drone).thetaToTarget);
    else if(role == R)
        return QVariant::fromValue(PTZ[index.row()]->vectorToDrone(drone).r);
    else if(role == DronePostion)
        return QVariant::fromValue(drone->getPosition());

    else if(role == DroneVector)
        return QVariant::fromValue(PTZ[index.row()]->caclculatedEndPoint(drone));

    else if(role == LeftLine)
        return QVariant::fromValue(PTZ[index.row()]->LeftViewPoint());

    else if(role == RightLine)
        return QVariant::fromValue(PTZ[index.row()]->RightViewPoint());

    else if(role == HalfLeftLine)
        return QVariant::fromValue(PTZ[index.row()]->HalfLeftViewPoint());

    else if(role == HalfRightLine)
        return QVariant::fromValue(PTZ[index.row()]->HalfRightViewPoint());

    return QVariant();
}

QHash<int, QByteArray> Marker::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Number] = "number";
    roles[MarkerPostion] = "position";
    roles[Vertex1] = "v1";
    roles[Vertex2] = "v2";
    roles[Vertex3] = "v3";
    roles[Radius] = "raduis";
    roles[CircleColor] = "circleColor";
    roles[CenterLine] = "centerLine";
    roles[Up] = "up";
    roles[ViewLine] = "viewLine";
    roles[Azimuth] = "azimuth";
    roles[Elevation] = "elevatoin";
    roles[Phi] = "phi";
    roles[Ro] = "ro";
    roles[Theta] = "theta";

    roles[CaclulatedLine] = "caclulatedLine";
    roles[DronePostion] = "dronePosition";
    roles[DroneVector] = "droneVector";
    roles[RightLine] = "rightLine";
    roles[LeftLine] = "leftLine";
    roles[R] = "r";
    roles[HalfRightLine] = "halfRightLine";
    roles[HalfLeftLine] = "halfLeftLine";

    return roles;
}

bool Marker::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

void Marker::update(int n ,int role){
    emit dataChanged(index(n),index(n),QVector<int>() << role);

}

void Marker::updateDrone(){

    emit dataChanged(index(0),index(0),QVector<int>() << DronePostion);
   // emit dataChanged(index(0),index(0),QVector<int>() << DroneVector);

}

void Marker::removeMarkerAll()
{
     beginRemoveRows(QModelIndex(),0,PTZ.count());
     PTZ.clear();
     endRemoveRows();

}
Qt::ItemFlags Marker::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

void Marker::addMarker(int row, PTZElement * p)
{
    beginInsertRows(QModelIndex(),row,row);
    PTZ.insert(row,p);
    endInsertRows();
}

void Marker::removeMarker(int row)
{
    beginRemoveRows(QModelIndex(),row,row);
    PTZ.removeAt(row);
    endRemoveRows();

}

QString Marker::current() const
{

    return "Current";
}

void Marker::setDrone(Drone *d)
{
    this->drone = d;
}
