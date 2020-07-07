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
