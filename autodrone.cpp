#include "autodrone.h"
#include <QDebug>
AutoDrone::AutoDrone(QObject *parent) : QObject(parent)
{

    for(int i = 0 ; i < 4; i++){
        latLonCurve.addPoint(i,{0,0});
        altCurve.addPoint(i,{0,0});
        speedCurve.addPoint(i,{0,0});
    }


    connect(&latLonCurve,SIGNAL(posChanged(int)),this,SLOT(geoCPChanged(int)));
    initInfoWidget();
    initScenes();
    initGraphicWidget();


}

QWidget *AutoDrone::getInfoWidget()
{
    return this->root;
}

void AutoDrone::initInfoWidget()
{
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


    root->setTitle("AutoDrone");
    lat->setText("Latitude : ");
    lng->setText("Longitude :");
    alt->setText("Altitude :");


}

QWidget *AutoDrone::getGraphicWidget()
{
    return view;
}

void AutoDrone::initGraphicWidget()
{
    view = new QGraphicsView();
    setScene(XY);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void AutoDrone::initScenes()
{
    QFont f("Times", 8, QFont::Thin); ;
    scene_xy = new QGraphicsScene();
    scene_z = new QGraphicsScene();
    scene_SpeedCurve = new QGraphicsScene();


    for(int i = 1 ; i <= 20;i++){
        float raduis = (float)(unitToMeter * circuitRaduis * i )/2.0f;
        scene_xy->addEllipse({-1*raduis,-1*raduis,raduis*2,raduis*2});
        scene_xy->addText(QString::number(raduis),f)->setPos(raduis - 10,2);
    }
    scene_xy->addItem(&latLonCurve.getPath());
    scene_xy->addItem(&latLonCurve.getM());
    scene_xy->addItem(&latLonCurve.getPointPath());
    //scene_xy->addRect(0,0,10,10,QPen("red"));
    for(auto v :latLonCurve.getControlPoints())
        scene_xy->addItem(v);



    scene_z->addItem(&altCurve.getPath());
    scene_z->addItem(&altCurve.getM());
    scene_z->addItem(&altCurve.getPointPath());
    for(int i = 0 ; i < 10;i++){
        float level = (float)(altTometer * i );

        scene_z->addLine(50,-1*level,-1*50,-1*level);

    }
    for(auto v :altCurve.getControlPoints())
        scene_z->addItem(v);



    scene_SpeedCurve->addItem(&speedCurve.getPath());
    scene_SpeedCurve->addItem(&speedCurve.getM());
    scene_SpeedCurve->addItem(&speedCurve.getPointPath());
    scene_SpeedCurve->addRect(0,0,10,10,QPen("red"));
    for(auto v :speedCurve.getControlPoints())
        scene_SpeedCurve->addItem(v);

}



void AutoDrone::setPathGeoCenter(QGeoCoordinate c)
{
    latLonCurve.setCenter(c);

}

void AutoDrone::advance(float t)
{
    latLonCurve.advanceGeoPoint(t);
    altCurve.advancePoint(t);
    speedCurve.advancePoint(t);

    updateInfoWidget();

}

void AutoDrone::setScene(AutoDrone::SceneType t)
{
    QGraphicsScene * sc = t == XY ? scene_xy : t == Z ? scene_z : scene_SpeedCurve;
    view->setScene(sc);
    view->setSceneRect(sc->sceneRect());

}

void AutoDrone::setTime(float t)
{
    latLonCurve.setTime(t);
    altCurve.setTime(t);
    speedCurve.setTime(t);

}

double AutoDrone::getLatitude()
{
    return latLonCurve.getGeoM().latitude();
}

double AutoDrone::getLongitude()
{
    return latLonCurve.getGeoM().longitude();

}

double AutoDrone::getAltitude()
{
    return -1*altCurve.getM().pos().y()/altTometer;

}

QGeoCoordinate AutoDrone::getPostition()
{
    QGeoCoordinate g = latLonCurve.getGeoM();
    g.setAltitude(getAltitude());
    return g;
}

void AutoDrone::geoCPChanged(int n)
{
    emit geoPosChanged(n);

}

void AutoDrone::advanceDefult()
{

    advance(0.01);



}

void AutoDrone::updateInfoWidget()
{

    lngEdit->setText(QString::number(getLatitude()));
    latEdit->setText(QString::number(getLatitude()));
    altEdit->setText(QString::number(getAltitude()));
}
