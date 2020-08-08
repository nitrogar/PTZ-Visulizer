#include "drone.h"
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QGeoCoordinate>
double Drone::getLatitude() const {
    return current.latitude();
}

void Drone::setLatitude(double latitude) {
    current.setLatitude(latitude);
    this->latEdit->setText(QString::number(latitude));
}

double Drone::getLongitude() const {
    return current.longitude();

}

void Drone::setLongitude(double longitude) {
    current.setLongitude(longitude);
    this->lngEdit->setText(QString::number(longitude));

}

double Drone::getAltitude() const {
    return current.altitude();
}

void Drone::setAltitude(double altitude) {
    current.setAltitude(altitude);
    this->altEdit->setText(QString::number(altitude));

}

double Drone::getXVec() const {
    return velocity.x_vec;
}

void Drone::setXVec(double xVec) {
    velocity.x_vec = xVec;
}

double Drone::getYVec() const {
    return velocity.y_vec;
}

void Drone::setYVec(double yVec) {
    velocity.y_vec = yVec;
}

double Drone::getZVec() const {
    return velocity.z_vec;
}

void Drone::setZVec(double zVec) {
    velocity.z_vec = zVec;
}

QGeoCoordinate Drone::getPosition()
{
    return current;
}

Drone::Drone(double latitude, double longitude, double altitude, double xVec, double yVec, double zVec) {
    current.setAltitude(altitude);
    current.setLongitude(longitude);
    current.setLatitude(latitude);
    velocity.x_vec = xVec;
    velocity.y_vec = yVec;
    velocity.z_vec = zVec;
}

Drone::Drone(double latitude, double longitude, double altitude):Drone(){
    current.setAltitude(altitude);
    current.setLongitude(longitude);
    current.setLatitude(latitude);
}

Drone::Drone() {
    this->root = new QGroupBox();
    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *v = new QVBoxLayout(root);
    QHBoxLayout *h1 = new QHBoxLayout();
    QLabel *lat = new QLabel(root);
    h1->addWidget(lat);

    this->latEdit = new QLineEdit(root);
    sizePolicy3.setHeightForWidth(latEdit->sizePolicy().hasHeightForWidth());
    latEdit->setSizePolicy(sizePolicy3);
    h1->addWidget(latEdit);

    v->addLayout(h1);


    QHBoxLayout* h2 = new QHBoxLayout();
    QLabel * lng = new QLabel(root);
    h2->addWidget(lng);

    this->lngEdit = new QLineEdit(root);
    sizePolicy3.setHeightForWidth(lngEdit->sizePolicy().hasHeightForWidth());
    lngEdit->setSizePolicy(sizePolicy3);
    h2->addWidget(lngEdit);

    v->addLayout(h2);

    QHBoxLayout* h3 = new QHBoxLayout();
    QLabel * alt = new QLabel(root);
    h3->addWidget(alt);

    this->altEdit = new QLineEdit(root);
    sizePolicy3.setHeightForWidth(altEdit->sizePolicy().hasHeightForWidth());
    altEdit->setSizePolicy(sizePolicy3);
    h3->addWidget(altEdit);

    v->addLayout(h3);


    root->setTitle("Drone");
    lat->setText("Latitude : ");
    lng->setText("Longitude :");
    alt->setText("Altitude :");


}

QWidget *Drone::getRoot()
{
    return this->root;
}
