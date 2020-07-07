#include "ptzelement.h"
#include <cmath>
PTZElement::PTZElement(int id)
{
            this->id = id;
            this->circleColor = "red";
            root = new QGroupBox();
            root->setObjectName(QString::fromUtf8("PTZElement"));
            root->setAlignment(Qt::AlignCenter);
            verticalLayout_9 = new QVBoxLayout(root);
            verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
            horizontalLayout = new QHBoxLayout();
            horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
            PTZlabelLat = new QLabel(root);
            PTZlabelLat->setObjectName(QString::fromUtf8("PTZlabelLat"));

            horizontalLayout->addWidget(PTZlabelLat);

            PTZLat = new QLineEdit(root);
            PTZLat->setObjectName(QString::fromUtf8("PTZLat"));
            QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
            sizePolicy3.setHorizontalStretch(0);
            sizePolicy3.setVerticalStretch(0);
            sizePolicy3.setHeightForWidth(PTZLat->sizePolicy().hasHeightForWidth());
            PTZLat->setSizePolicy(sizePolicy3);

            horizontalLayout->addWidget(PTZLat);


            verticalLayout_9->addLayout(horizontalLayout);

            horizontalLayout_18 = new QHBoxLayout();
            horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
            PTZlabelLong = new QLabel(root);
            PTZlabelLong->setObjectName(QString::fromUtf8("PTZlabelLong"));

            horizontalLayout_18->addWidget(PTZlabelLong);

            PTZLong = new QLineEdit(root);
            PTZLong->setObjectName(QString::fromUtf8("PTZLong"));
            sizePolicy3.setHeightForWidth(PTZLong->sizePolicy().hasHeightForWidth());
            PTZLong->setSizePolicy(sizePolicy3);

            horizontalLayout_18->addWidget(PTZLong);


            verticalLayout_9->addLayout(horizontalLayout_18);

            horizontalLayout_19 = new QHBoxLayout();
            horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
            PTZlabelAz = new QLabel(root);
            PTZlabelAz->setObjectName(QString::fromUtf8("PTZlabelAz"));

            horizontalLayout_19->addWidget(PTZlabelAz);

            PTZAzimuth = new QLineEdit(root);
            PTZAzimuth->setObjectName(QString::fromUtf8("PTZAzimuth"));
            sizePolicy3.setHeightForWidth(PTZAzimuth->sizePolicy().hasHeightForWidth());
            PTZAzimuth->setSizePolicy(sizePolicy3);

            horizontalLayout_19->addWidget(PTZAzimuth);


            verticalLayout_9->addLayout(horizontalLayout_19);

            horizontalLayout_9 = new QHBoxLayout();
            horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
            PTZlabelEl = new QLabel(root);
            PTZlabelEl->setObjectName(QString::fromUtf8("PTZlabelEl"));

            horizontalLayout_9->addWidget(PTZlabelEl);

            PTZElevation = new QLineEdit(root);
            PTZElevation->setObjectName(QString::fromUtf8("PTZElevation"));
            sizePolicy3.setHeightForWidth(PTZElevation->sizePolicy().hasHeightForWidth());
            PTZElevation->setSizePolicy(sizePolicy3);

            horizontalLayout_9->addWidget(PTZElevation);


            verticalLayout_9->addLayout(horizontalLayout_9);



            horizontalLayout_10 = new QHBoxLayout();
            PTZlabelAlt = new QLabel(root);
            horizontalLayout_10->addWidget(PTZlabelAlt);

            PTZAltitude = new QLineEdit(root);
            sizePolicy3.setHeightForWidth(PTZAltitude->sizePolicy().hasHeightForWidth());
            PTZAltitude->setSizePolicy(sizePolicy3);
            horizontalLayout_10->addWidget(PTZAltitude);

            verticalLayout_9->addLayout(horizontalLayout_10);


            horizontalLayout_11 = new QHBoxLayout();
            PTZlabelRange = new QLabel(root);
            horizontalLayout_11->addWidget(PTZlabelRange);

            PTZRange = new QLineEdit(root);
            sizePolicy3.setHeightForWidth(PTZRange->sizePolicy().hasHeightForWidth());
            PTZRange->setSizePolicy(sizePolicy3);
            horizontalLayout_11->addWidget(PTZRange);

            verticalLayout_9->addLayout(horizontalLayout_11);



            horizontalLayout_12 = new QHBoxLayout();
            PTZlabelcenterLine = new QLabel(root);
            horizontalLayout_12->addWidget(PTZlabelcenterLine);

            PTZCenterLine = new QLineEdit(root);
            sizePolicy3.setHeightForWidth(PTZCenterLine->sizePolicy().hasHeightForWidth());
            PTZCenterLine->setSizePolicy(sizePolicy3);
            horizontalLayout_12->addWidget(PTZCenterLine);

            verticalLayout_9->addLayout(horizontalLayout_12);





            horizontalLayout_13 = new QHBoxLayout();
            PTZlabelPhi = new QLabel(root);
            horizontalLayout_13->addWidget(PTZlabelPhi);

            PTZPhi = new QLineEdit(root);
            sizePolicy3.setHeightForWidth(PTZPhi->sizePolicy().hasHeightForWidth());
            PTZPhi->setSizePolicy(sizePolicy3);
            horizontalLayout_13->addWidget(PTZPhi);

            verticalLayout_9->addLayout(horizontalLayout_13);



            horizontalLayout_14 = new QHBoxLayout();
            PTZlabelRef = new QLabel(root);
            horizontalLayout_14->addWidget(PTZlabelRef);

            PTZRef = new QLineEdit(root);
            sizePolicy3.setHeightForWidth(PTZRef->sizePolicy().hasHeightForWidth());
            PTZRef->setSizePolicy(sizePolicy3);
            horizontalLayout_14->addWidget(PTZRef);

            verticalLayout_9->addLayout(horizontalLayout_14);



            root->setTitle("PTZ - " + QString::number(id));
            PTZlabelLat->setText("Latitude : ");
            PTZlabelLong->setText("Longitude :");
            PTZlabelAz->setText("Azimuth Angle :");
            PTZlabelEl->setText("Elevation Angle :");
            PTZlabelcenterLine->setText("View Line :");
            PTZlabelRange->setText("View Range :");
            PTZlabelAlt->setText("Altitude :");
            PTZlabelRef->setText("Reference Azimuth :");
            PTZlabelPhi->setText("Referce Elevation :");
            root->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);

}
QGroupBox * PTZElement::getRoot(){
    return this->root;
}

QGeoCoordinate PTZElement::getViewLine()
{
    return FindEndPoint(this->azimuth,range);
}

PTZElement::Vector3d PTZElement::getVectorTo(double lat, double lng, double alt)
{
    double r = this->haversine(lat,lng,this->latitude,this->longitude);
    if(r > this->range) return {0,0,0};
    double n = this->haversine(this->latitude,this->longitude,this->longitude,lat);

    double th0 = acos(r/n);
    double dlat = this->latitude - lat;
    double dlng = this->longitude - lng;

    if(dlat < 0 && dlng < 0 ) th0 =(M_PI_2 - th0) +  M_PI;
    else if(dlat > 0 && dlng < 0) th0 += M_PI_2;
    else if(dlat < 0 && dlng > 0) th0 += M_PI/3;
    else th0 -= M_PI_2;


    double th1 = this->angleFromNorth * DEG_TO_RAD;
    th1 = th0 - th1;

    if(th1 < 0) th1 += M_PI;

    double dalt = alt - this->altitude;
    double phi;

    if(dalt == 0) phi = 0;

    phi = atan(r/dalt);
    if(dalt < 0) phi = M_PI_2 + (M_PI_2 - phi);

    double ro = r / sin(phi);

    if(ro > range) return {0,0,0};

    return {th1,phi,ro};

}

float PTZElement::haversine(double lat0, double lng0 ,double lat1, double lng1 )
{
    double radLat0 = (lat0) * DEG_TO_RAD;
    double radLat1 = (lat1) * DEG_TO_RAD;

     double dlat = (lat0 - lat1) * DEG_TO_RAD;
    double dlng = (lng0 - lng1) * DEG_TO_RAD;

    double a = powf(sin(dlat/2),2.0f)+cos(radLat0)*cos(radLat1)+powf(sin(dlng/2),2.0f);
    double c = 2 * atan2(sqrt(a),sqrt(1-a));

    return EARTH_RADIUS_IN_METERS * c;


}
void PTZElement::setLat(float lat){
    this->latitude = lat;
    this->PTZLat->setText(QString::number(lat));

}
void PTZElement::setLong(float lng){
    this->longitude = lng;
    this->PTZLong->setText(QString::number(lng));

}
void PTZElement::setAzimuthAngle(float angle){
    this->azimuth = angle;
    this->PTZAzimuth->setText(QString::number(angle));

}
void PTZElement::setElevationAngle(float angle){
    this->elevation = angle;
    this->PTZElevation->setText(QString::number(angle));

}
void PTZElement::setAlt(float alt){
    this->altitude = alt;
    this->PTZAltitude->setText(QString::number(alt));

}

void PTZElement::setRange(float r){
    this->range = r;
    this->PTZRange->setText(QString::number(r));

}
void PTZElement::setref(float ref){
    this->angleFromNorth = ref;
    this->PTZRef->setText(QString::number(ref));

}
void PTZElement::setPhi(float phi){
    this->Phi = phi;
    this->PTZPhi->setText(QString::number(phi));

}
float PTZElement::getLat(){

    return this->PTZLat->text().toFloat();
}
float PTZElement::getLong(){
    return this->PTZLong->text().toFloat();

}
float PTZElement::getElevationAngle(){
    return this->elevation;

}
float PTZElement::getAzimuthAngle(){
    return this->azimuth;
}

QGeoCoordinate PTZElement::getPostion()
{
    return QGeoCoordinate(this->latitude,this->longitude);
}

QGeoCoordinate PTZElement::getEndLine()
{
    return FindEndPoint(0,range);

    /*
    double delta = this->range/EARTH_RADIUS_IN_METERS;
    double lat = this->latitude * DEG_TO_RAD;
    double lng = this->longitude * DEG_TO_RAD;
    double th0 = this->angleFromNorth * DEG_TO_RAD;

    double lat2 = asin(sin(lat)*cos(delta) + cos(lat)*sin(delta)*cos(th0));
    lat2 *= 180/M_PI;

    double lng2 = lng + atan2(sin(th0)*sin(delta)*cos(lat),cos(delta)-sin(lat)*sin(lat2));
    lng2 *= 180/M_PI;
    return QGeoCoordinate(lat2,lng2);

    */

}

QGeoCoordinate PTZElement::FindEndPoint(double angle,double r){

    double delta = this->range/EARTH_RADIUS_IN_METERS;
    double lat = this->latitude * DEG_TO_RAD;
    double lng = this->longitude * DEG_TO_RAD;
    double th0 = angle ;//this->angleFromNorth - angle;
   // if(th0 < 0) th0 += 360;
    th0 *= DEG_TO_RAD;

    double lat2 = asin(sin(lat)*cos(delta) + cos(lat)*sin(delta)*cos(th0));
    lat2 *= 180/M_PI;

    double lng2 = lng + atan2(sin(th0)*sin(delta)*cos(lat),cos(delta)-sin(lat)*sin(lat2));
    lng2 *= 180/M_PI;
    return QGeoCoordinate(lat2,lng2);

}

