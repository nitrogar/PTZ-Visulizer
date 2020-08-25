#include "ptzelement.h"
#include <cmath>
#include "drone.h"
#include <QDebug>
#include "autodrone.h"
#include <QValueAxis>
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




            horizontalLayout_15 = new QHBoxLayout();
            PTZlabelTargetAzimuth = new QLabel(root);
            horizontalLayout_15->addWidget(PTZlabelTargetAzimuth);

            PTZTargetAzimuth = new QLineEdit(root);
            sizePolicy3.setHeightForWidth(PTZTargetAzimuth->sizePolicy().hasHeightForWidth());
            PTZTargetAzimuth->setSizePolicy(sizePolicy3);
            horizontalLayout_15->addWidget(PTZTargetAzimuth);

            verticalLayout_9->addLayout(horizontalLayout_15);


            horizontalLayout_16 = new QHBoxLayout();
            PTZlabelTargetElevation = new QLabel(root);
            horizontalLayout_16->addWidget(PTZlabelTargetElevation);

            PTZTargetElevation = new QLineEdit(root);
            sizePolicy3.setHeightForWidth(PTZTargetElevation->sizePolicy().hasHeightForWidth());
            PTZTargetElevation->setSizePolicy(sizePolicy3);
            horizontalLayout_16->addWidget(PTZTargetElevation);

            verticalLayout_9->addLayout(horizontalLayout_16);



            root->setTitle("PTZ - " + QString::number(id));
            PTZlabelLat->setText("Latitude : ");
            PTZlabelLong->setText("Longitude :");
            PTZlabelAz->setText("Azimuth Angle :");
            PTZlabelEl->setText("Elevation Angle :");
            PTZlabelcenterLine->setText("FOV :");
            PTZlabelRange->setText("View Range :");
            PTZlabelAlt->setText("Altitude :");
            PTZlabelRef->setText("Reference Azimuth :");
            PTZlabelPhi->setText("Referce Elevation :");
            PTZlabelTargetElevation->setText("Target Elevation :");
            PTZlabelTargetAzimuth->setText("Target Azimuth :");
            root->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);


            initScene();
            initChart();


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
    double th0;
    if(r > this->range) return {0,0,0,0};
    double n = this->haversine(this->latitude,this->longitude,lat,this->longitude);

    if(n == 0 && lng >this->longitude) th0 = M_PI_2;
    else if(n == 0 && lng < this->longitude) th0 = M_PI + M_PI_2;
    else {
        th0 = acos(n/r);
    }

    double dlat = this->latitude - lat;
    double dlng = this->longitude - lng;

    if(dlat > 0 && dlng < 0) th0 = M_PI - th0;
    else if(dlat < 0 && dlng > 0) th0 = 2*M_PI - th0;
    else if(dlat > 0 && dlng > 0 ) th0 += M_PI;


    double dalt = alt - this->altitude;
    double phi;

    if(dalt == 0) phi = 0;

    phi = atan2(dalt,r);
    phi = M_PI_2 - phi;

    double ro = r / sin(phi);
    if(ro > range || (ro < 0 && -1*ro > range)) return {0,0,0,0};


    return {th0 * 180/M_PI,phi * 180/M_PI,ro,r};

}

float PTZElement::haversine(double lat0, double lng0 ,double lat1, double lng1 )
{
    double radLat0 = (lat0) * DEG_TO_RAD;
    double radLat1 = (lat1) * DEG_TO_RAD;

     double dlat = (lat1 - lat0) * DEG_TO_RAD;
    double dlng = (lng1 - lng0) * DEG_TO_RAD;

    double a = powf(sin(dlat/2),2.0f)+cos(radLat0)*cos(radLat1)*powf(sin(dlng/2),2.0f);
    double c = 2 * atan2(sqrt(a),sqrt(1-a));

    return EARTH_RADIUS_IN_METERS * c;


}
// deprecated
QGeoCoordinate PTZElement::positionAt(AutoDrone *d, float t)
{
    QVector3D final = positionXYZAt(d,t);
    return cartToGeo(final);

}

QVector3D PTZElement::positionXYZAt(AutoDrone *d, float t)
{
    QVector3D v = velocity(t);
    QGeoCoordinate dronePos = d->getPostition();
    QVector3D pos_xyz = GeoTocart(dronePos);
    QVector3D final = v+pos_xyz;
    return final;
}
// deprecated
QGeoCoordinate PTZElement::cartToGeo(QVector3D &v)
{
    float dist_x = v.x();
    float dist_y = v.y();
    float dist_z = v.z();

    float angle = atan2(dist_x,dist_y) * 180/M_PI;
    float bearing = angle > 0 ? angle : angle + 360;
    float dist = sqrt(dist_x*dist_x + dist_y *dist_y);

    QGeoCoordinate position = QGeoCoordinate(latitude,longitude,dist_z).atDistanceAndAzimuth(dist,bearing);
    return position;
}

QVector3D PTZElement::GeoTocart(QGeoCoordinate &g)
{
    QGeoCoordinate center = {latitude,longitude,altitude};

    float dist_x = center.distanceTo(QGeoCoordinate(center.latitude(),g.longitude()));

    float bearing = center.azimuthTo(g);
    float dist_y = center.distanceTo(QGeoCoordinate(g.latitude(), center.longitude()));
    QGeoCoordinate x = QGeoCoordinate(center.latitude(),g.longitude());

    if(bearing > 90 && bearing < 270) dist_y *=-1;
    if(bearing > 180) dist_x *=-1;


    float dist_z = g.altitude() - center.altitude();
    //qDebug() << QString("g : %0 center: %1").arg(g.altitude()).arg(center.altitude());
    return {dist_x,dist_y,dist_z};
}

// will be renamed as distacneTraveld
QVector3D PTZElement::velocity(float t)
{

    return {param[0] * t ,param[1] * t,param[2] * t};

}
PTZElement::Factors PTZElement::caclulateFastes(AutoDrone *d,float dist,float zdist)
{
    QVector<QVector3D> pred,timeNeeded;
    QVector<int> candidate;
    QVector<float> timeTo;
    QVector<QPointF> angles;
    QVector3D tmp ;
    QPointF ptm,p;
    float Azmfactor = -1,Elefactor = -1;
    float shAngle ;
    float cost, maxCost,minCost,costAzm,costEle,azmSpeed,EleSpeed,droneTimeToPoint,AzmTimeToPoint,EleTimeToPoint,XYSpeed;
    float time,maxTime,minTime,tmp2,tmp3;
    bool azmready = false,eleready = false;


    isfirstPointInPredection = true;

    for(int i = 1; i < 20; i++){
        time = i * 0.1;
        tmp =  positionXYZAt(d,time);
        p = QPointF(tmp.x(),-tmp.y());

        tmp2 = atan2(tmp.x(),tmp.y()) * 180/M_PI;
        tmp3 = tmp2 < 0 ? tmp2+360:tmp2;
        tmp2 = atan2(tmp.z(),sqrt(tmp.x()*tmp.x()+tmp.y()*tmp.y()))* 180/M_PI;
        addPredictPoint(p);
        //pred << tmp;
        timeTo << time;
        shAngle = tmp3 - azimuth ;
        if(shAngle < -180) shAngle += 360;
        if(shAngle > 180) shAngle -= 360;

        ptm = QPointF(shAngle,tmp2 - elevation);
        angles << ptm;
    }

   minCost = 10000000;
   //float azimuthAnglurSpeed = 50;
   //float elevationAnglurSpeed = 50;
   double pret1 ,t1,t2,t ,diffAngle;
   double *ts;
   float b1,b2,db,ZSpeed;
   QPointF a = {historyXYZ[historyXYZ.size()-2].x(),historyXYZ[historyXYZ.size()-2].z()},b={historyXYZ[historyXYZ.size()-1].x(),historyXYZ[historyXYZ.size()-1].z()};
   float O = calculateOrbitalVelocity(1,historyXYZ[historyXYZ.size()-2].toPointF(),historyXYZ[historyXYZ.size()-1].toPointF());
   float O2 = calculateOrbitalVelocity(0,a,b);
   if(isnanf(O2)){

       qDebug() << "No use " ;

   }
   // this line in the function above
   float OrbitalSpeed = sqrt(param[0]*param[0] + param[1]*param[1])*sin(O)/dist;
   XYSpeed = OrbitalSpeed * 180/M_PI; ////sqrt(param[0]*param[0] + param[1]*param[1])*sin(db)/dist;
   float Z = sqrt(param[0]*param[0] + param[2]*param[2])*sin(O2)/zdist;
   ZSpeed = Z * 180/M_PI;
   float maxAzm , maxEle;
   for(int i = 0 ; i < angles.size(); i++){

                droneTimeToPoint = timeTo[i];


                maxAzm = angles[i].x() < 0 ? -1*maxAzimuthAnglurSpeed : maxAzimuthAnglurSpeed;
                maxEle = angles[i].y() < 0 ? -1*maxElevationAnglurSpeed : maxElevationAnglurSpeed;


                XYSpeed = isnanf(XYSpeed) ? abs(angles[i].x()/droneTimeToPoint) : XYSpeed;
                XYSpeed = abs(XYSpeed) > abs(maxAzm)  ? maxAzm : XYSpeed;

                ZSpeed = isnanf(ZSpeed) ? abs(angles[i].y()/droneTimeToPoint) : ZSpeed;
                ZSpeed = abs(ZSpeed) > abs(maxEle)  ? maxEle : ZSpeed;

               // if(isnanf(O2)){

                 //   qDebug() << "No use " ;
                 //ZSpeed = abs(angles[i].y()/droneTimeToPoint);
                   // qDebug() << QString("%0 %1 %2").arg(angles[i].y()).arg(droneTimeToPoint).arg(elevation);

                //}
               //                // qDebug() << QString("%0 %1 %2").arg(angles[i].y()).arg(droneTimeToPoint).arg(elevation);
              //  qDebug() << angles;
                diffAngle = abs(angles[i].x()/180);
                t = NAN;
                ts = sloveCubic(1.3,-0.6,0.3,0,diffAngle);
                for(int i = 0 ; i < 3 ; i++){

                    if(abs(ts[i]) <= 1 && ts[i] != NAN) {t = ts[i];}
                }
                if(t == NAN) qDebug() << QString("No solution %0").arg(diffAngle);

                azmSpeed = (1-t)*(1-t)*(1-t)*XYSpeed + 3*(1-t)*(1-t)*t*XYSpeed +3*(1-t)*t*t*maxAzm + t*t*t*maxAzm;



                diffAngle = abs(angles[i].y()/180);
                t = NAN;
                ts = sloveCubic(1.3,-0.6,0.3,0,diffAngle);
                for(int i = 0 ; i < 3 ; i++){

                    if(abs(ts[i]) <= 1 && ts[i] != NAN) {t = ts[i];}
                }
                if(t == NAN) qDebug() << QString("No solution El %0").arg(diffAngle);

                //t2 = abs(angles[i].y()/180);
                //t = t2;
                EleSpeed = (1-t)*(1-t)*(1-t)*ZSpeed + 3*(1-t)*(1-t)*t*ZSpeed +3*(1-t)*t*t*maxEle + t*t*t*maxEle;


                AzmTimeToPoint = abs(angles[i].x() / azmSpeed);
                EleTimeToPoint = abs(angles[i].y() / EleSpeed);

                qDebug() << QString("[%0] droneTime: %1 azmTime: %2 eleTime: %3 diffAng Azm: %4 Ele: %5 Speeds XY: %6 AzmSpeed: %7 ZSpeed: %8 EleSpeed: %9 ")
                            .arg(i).arg(timeTo[i]).arg(AzmTimeToPoint).arg(EleTimeToPoint).arg(angles[i].x()).arg(angles[i].y()).arg(XYSpeed).arg(azmSpeed).arg(ZSpeed).arg(EleSpeed);

                if(droneTimeToPoint < AzmTimeToPoint && droneTimeToPoint < EleTimeToPoint ) continue;


               // Azmfactor = angles[i].x() < 0 ? -1*azmSpeed : azmSpeed;
               // Elefactor = angles[i].y() < 0 ? -1*EleSpeed : EleSpeed;
               /// Delete ME //
               Azmfactor = azmSpeed;
               Elefactor = EleSpeed;

                if(abs(angles[i].x()) < 1 && ((oldAzimuth < 0 && angles[i].x() > 0) || (oldAzimuth > 0 && angles[i].x() < 0))){ qDebug() << QString("AZViprate"); Azmfactor = 0;}
                if(abs(angles[i].y()) < 1 && ((oldElevation < 0 && angles[i].y() > 0) || (oldElevation > 0 && angles[i].y() < 0))){ qDebug() << QString("ELViprate"); Elefactor = 0;}


                oldAzimuth = angles[i].x();
                oldElevation = angles[i].y();
                break;
   }



   if(elevationAnglurSpeed != 0) {

        errElevationSpeedSerise.append(total_time,abs((ZSpeed - elevationAnglurSpeed)/elevationAnglurSpeed)*100 );
        qDebug() << QString("$Error : %0 , Z  : %1 , current ele : %2 diff : %3  future ele : %4 t : %5")
                    .arg((ZSpeed - elevationAnglurSpeed)/elevationAnglurSpeed  *100).arg(ZSpeed).arg(elevationAnglurSpeed).arg((ZSpeed - elevationAnglurSpeed)).arg(Elefactor).arg(total_time);;

        if(abs((ZSpeed - elevationAnglurSpeed)/elevationAnglurSpeed )> 2) QString("!!!!!!!ELE!!!!!!!!");
    }
    if(azimuthAnglurSpeed != 0)  {

        errSpeedSerise.append(total_time,abs((XYSpeed - azimuthAnglurSpeed)/azimuthAnglurSpeed ) * 100);
        qDebug() << QString("$Error : %0 , XY : %1 , current azm : %2 diff : %3  future azm : %4 t : %5")
                    .arg((XYSpeed - azimuthAnglurSpeed)/azimuthAnglurSpeed *100).arg(XYSpeed).arg(azimuthAnglurSpeed).arg((XYSpeed - azimuthAnglurSpeed)).arg(Azmfactor).arg(total_time);

        if(abs((XYSpeed - azimuthAnglurSpeed)/azimuthAnglurSpeed) > 2) QString("!!!!!!!AZM!!!!!!!!");
    }

  qDebug() << QString("[#] azm: %0 ele:%1").arg(Azmfactor).arg(Elefactor) ;


    return {Azmfactor,Elefactor};

}

float  PTZElement::calculateBearing(float x , float y){

    float b = atan2(x,y) * 180/M_PI;
    return b < 0 ? b+360:b;
}

float PTZElement::calculateOrbitalVelocity(int isBearing, QPointF a, QPointF b)
{

    // equation of the line from a to o, y = mx + b ==> y = mx
    // b = 0 y-intercept m = a.y/a.x
    // in the stander form Ax + By = C , A = -m, B = 1 , C = 0;

    float
     m = a.y()/a.x(),
     // second equation for the prependicular line
     // y - y0 =  n(x - x0) ,
     // y =  y0 + nx -nx0
     // y =  nx + (y0 - nx0)

     // mx = nx + (y0 - nx0)
     // x(m-n) = (y0 -nx0)
     // x = (y0 - nx0)/(m - n)
     n = -1/m,
     x = (b.y() - n*b.x())/(m - n),
     y = m*x,
     difXab = a.x() - b.x(),
     difYab = a.y() - b.y(),
     diffXbc = b.x() - x,
     diffYbc = b.y() - y,
     distAB = sqrt(difXab*difXab + difYab*difYab),
     distBC = sqrt(diffXbc*diffXbc + diffYbc*diffYbc),
     thetaOA,thetaOB;
    if(isBearing){
        thetaOA= atan2(a.y(),a.x());
        thetaOB = atan2(b.y(),b.x());
    }
    else{
        thetaOA = atan2(a.x(),a.y());
        thetaOB = atan2(b.x(),b.y());

    }
     int clockwise = thetaOB > thetaOA ? -1 : 1;

   qDebug() << QString("a:(%4,%5) b:(%6,%7) m = %0 n = %1 x = %2 y = %3").arg(m).arg(n).arg(x).arg(y).arg(a.x()).arg(a.y()).arg(b.x()).arg(b.y());
    return clockwise * asin(distBC / distAB);


}

PTZElement::Factors  PTZElement::droneScan(AutoDrone *d, float timeElapsed)
{
    Vector3d v = vectorToDrone(d);
    if(v.isNull()){
        droneEndpoint = QGeoCoordinate(this->latitude,this->longitude);
        total_time = 0;


        return {0,0};
    }
    else
        droneEndpoint = QGeoCoordinate(this->latitude,this->longitude).atDistanceAndAzimuth(v.r , v.thetaToTarget);


    if(total_time == 0) {
        errAzimuthSerise.clear();
        errElevationSerise.clear();
        errSpeedSerise.clear();
        errElevationSpeedSerise.clear();
        painterPath.clear();

    }
    QGeoCoordinate dronePostion = d->getPostition();
    QVector3D xy = GeoTocart(dronePostion);

    historyXYZ << xy;
    historyGeo << dronePostion;

    if(historyXYZ.size() < 4) return {0,0};

    updateVelocityParameters(timeElapsed,v.r);
    //float errorAzimuth = azimuth - calculateBearing(xy.x(),xy.y());
    //float errorElevation = elevation - atan2(xy.z(),sqrt(xy.x()*xy.x()+xy.y()*xy.y()))* 180/M_PI;

    Factors f = caclulateFastes(d,v.r,v.roToTarget);

    total_time += timeElapsed/1000.0f ;
    AX.setMax(total_time);
    AX2.setMax(total_time);
    float dAzimuth = abs(v.thetaToTarget - azimuth);
    dAzimuth = dAzimuth > 180 ? 360 - dAzimuth : dAzimuth;
    errAzimuthSerise.append(total_time,dAzimuth);
    errElevationSerise.append(total_time,abs(v.phiToTarget - (90 - elevation)));

    qDebug() << QString("AzmDiff : %0 EleDiff : %1").arg(dAzimuth).arg(abs(v.phiToTarget - (90 - elevation)));

  // qDebug() << QString("Total_time %0 %1 %2").arg(total_time).arg(v.phiToTarget).arg(elevation);
  // qDebug() << errElevationSerise.points();



    return f;

}

// remove dist
void PTZElement::updateVelocityParameters(float elapsed,float dist)
{
    if(historyXYZ.size() < 2) return ;
    int size = historyXYZ.size();
    QVector3D dif = historyXYZ[size - 1] - historyXYZ[size -2];
    //qDebug() << QString("updateVelocityParmeter Different : ") << historyXYZ[size -2] << QString("-") <<  historyXYZ[size - 1] << QString("=") << dif;
    dif = dif / (elapsed/1000);

    param[0]=dif.x(); param[1]= dif.y() ; param[2] = dif.z();

    //qDebug() << QString("Param : ") << param;


}

bool PTZElement::approx(float a ,float b){
    return abs(a-b) < 0.00001;
}

// ax^3 + bx^2 + cx + d = e
double PTZElement::cubeRoot(double x){
    return x < 0 ? -pow(x,1.f/3.f) : pow(x,1.f/3.f);
}
double *  PTZElement::sloveCubic(float a,float b,float c,float d,float e)
{
    // to the form x^3 + a1x^2 + b1x + c1 = 0
   float
   a1 = b/a,
   b1 = c/a,
   c1 = (d-e)/a,
   p = (3*b1-a1*a1)/3,
   q = (2*a1*a1*a1 - 9*a1*b1 + 27*c1)/27,
   discriminant = q*q/4 + p*p*p/27,
   u , v;
   if(p == 0) return new double[3]{cubeRoot(-q),NAN,NAN};
   else if(q == 0) return new double[3]{NAN,NAN,NAN};
   if (discriminant < 0) {
       double
         np3 = -p/3,
         r = sqrt(np3*np3*np3),
         t = -q / (2 * r),
         cosphi = t < -1 ? -1 : t > 1 ? 1 : t,
         phi = acos(cosphi),
         r3 = cubeRoot(r),
         t1 = 2 * r3;


       return new double[3]{
         t1 * cos(phi / 3) - a1/3,
         t1 * cos((phi + 2*M_PI) / 3) - a1/3,
         t1 * cos((phi + 4*M_PI) / 3) - a1/3
       };
   }

     // case 2: three real roots, but two form a "double root",
     // and so will have the same resultant value. We only need
     // to return two values in this case.
     else if (discriminant == 0) {
       u = -q/2 < 0 ? -pow(-q/2,1.f/3.f) : pow(-q/2, 1.f/3.f);
       return new double[3]{
         2 * u - a1/3,
         -u - a1/3,
         NAN
       };
     }

     // case 3: one real root, 2 complex roots. We don't care about
     // complex results so we just ignore those and directly compute
     // that single real root.
     else {
       double sd = sqrt(discriminant);
       u = cubeRoot(-q/2 + sd);
       v = cubeRoot(q/2 + sd);
       return new double[3]{u - v - a1/3,NAN,NAN};
     }

}

float PTZElement::calculateDistance(QGeoCoordinate &a, QGeoCoordinate &b)
{
    float dlat = (a.latitude() - b.latitude());
    float y_dist = dlat* 111300;
    float x_dist = (a.longitude() - b.longitude()) * 111300 * cos(dlat/2  * M_PI/180);

    return sqrt(y_dist*y_dist + x_dist*x_dist);
}

void PTZElement::initScene()
{

    QPen pen1,pen2;
    pen1.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen1.setStyle(Qt::PenStyle::DotLine);
    pen1.setColor(QColor("gold"));
    pen1.setWidth(2);
    predictedPath.setPen(pen1);
    pen2.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen2.setStyle(Qt::PenStyle::SolidLine);
    pen2.setWidth(2);
    pen2.setColor(QColor("red"));
    dronePath.setPen(pen2);
    //scene.addItem(&dronePath);
    scene.addItem(&predictedPath);
}



void PTZElement::initChart()
{
      errAzimuthSerise.setName("Azimuth Error (degree)");
      errElevationSerise.setName("Elevation Error (degree)");
      errSpeedSerise.setName("Azimuth Speed Error (%)");
      errElevationSpeedSerise.setName("Elevation Speed Error (%)");

      AX.setRange(0,100);
      AY.setRange(0,360);

      AY.setTickCount(10);
      AX.setTickCount(10);
      rmsChart.addAxis(&AX, Qt::AlignBottom);
      rmsChart.addAxis(&AY, Qt::AlignLeft);
      rmsChart.addSeries(&errAzimuthSerise);
      rmsChart.addSeries(&errElevationSerise);


      rmsChart.setTitle("Tracking Errors");

      errAzimuthSerise.attachAxis(&AX);
      errAzimuthSerise.attachAxis(&AY);
      errElevationSerise.attachAxis(&AX);
      errElevationSerise.attachAxis(&AY);



      AX2.setRange(0,100);
      AY2.setRange(0,360);
      AY2.setTickCount(10);
      AX2.setTickCount(10);
      rmsSpeedChart.setTitle("Tracking Errors");
      rmsSpeedChart.addAxis(&AX2, Qt::AlignBottom);
      rmsSpeedChart.addAxis(&AY2, Qt::AlignLeft);
      rmsSpeedChart.addSeries(&errSpeedSerise);
      rmsSpeedChart.addSeries(&errElevationSpeedSerise);

      errSpeedSerise.attachAxis(&AX2);
      errSpeedSerise.attachAxis(&AY2);
      errElevationSpeedSerise.attachAxis(&AX2);
      errElevationSpeedSerise.attachAxis(&AY2);
}

void PTZElement::addPredictPoint(QPointF &p)
{
    if(isfirstPointInPredection){
        painterPath.moveTo(p);
        isfirstPointInPredection = false;
    }
    else
        painterPath.lineTo(p);

    predictedPath.setPath(painterPath);

}

void PTZElement::addRealPoint(QPointF &p)
{
    dronePainterPath.lineTo(p);
    dronePath.setPath(dronePainterPath);

}
void PTZElement::clearPrePath()
{
    painterPath.clear();

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

void PTZElement::setTargetAzimuthAngle(float angle){
    this->PTZTargetAzimuth->setText(QString::number(angle));
}


void PTZElement::setTargetElevationAngle(float angle){
    this->PTZTargetElevation->setText(QString::number(angle));
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
    QFont f("Times", 8, QFont::Thin); ;

   float raduis = (float)(range);
   scene.addEllipse({-1*raduis,-1*raduis,raduis*2,raduis*2});
   scene.addText(QString::number(raduis),f)->setPos(raduis - 10,2);
    this->PTZRange->setText(QString::number(r));

}
void PTZElement::setref(float ref){
    this->angleFromNorth = ref;
    this->PTZRef->setText(QString::number(ref));

}

void PTZElement::setFOV(float fov)
{
    this->fov = fov;
    this->PTZCenterLine->setText(QString::number(fov));

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

QGeoCoordinate PTZElement::LeftViewPoint()
{
    return FindEndPoint(this->azimuth - this->fov/2.0f,this->range);

}

QGeoCoordinate PTZElement::RightViewPoint()
{
    return FindEndPoint(this->fov/2.0f + this->azimuth,this->range);
}

QGeoCoordinate PTZElement::HalfLeftViewPoint()
{
    return FindEndPoint(this->azimuth - this->fov/4.0f,this->range);

}

QGeoCoordinate PTZElement::HalfRightViewPoint()
{
    return FindEndPoint(this->fov/4.0f + this->azimuth,this->range);

}

PTZElement::Vector3d PTZElement::vectorToDrone(AutoDrone *d)
{
    return getVectorTo(d->getLatitude(),d->getLongitude(),d->getAltitude());
   // raw.phiToTarget = 90 - raw.phiToTarget;
   // return raw;
}

// will be renamed as calculateDroneVectorEndPoint
QGeoCoordinate PTZElement::caclculatedEndPoint(AutoDrone * drone)
{
    int id = this->id;
    Vector3d vec = vectorToDrone(drone);
    //qDebug() << QString("id : %1 Th0 : %2 Phi : %3 Ro : %4 R : %5").arg(this->id).arg(vec.thetaToTarget).arg(vec.phiToTarget).arg(vec.roToTarget).arg(vec.r);


    if(vec.thetaToTarget == 0 && vec.roToTarget == 0 && vec.phiToTarget == 0)
        return QGeoCoordinate(this->latitude,this->longitude);

    //qDebug() << QString("id : %1 Th0 : %2 Phi : %3 Ro : %4 R : %5").arg(this->id).arg(vec.thetaToTarget).arg(vec.phiToTarget).arg(vec.roToTarget).arg(vec.r);

    return QGeoCoordinate(this->latitude,this->longitude).atDistanceAndAzimuth(vec.r , vec.thetaToTarget);//FindEndPoint(vec.thetaToTarget,vec.r);




}

QGeoCoordinate PTZElement::getPostion()
{
    return QGeoCoordinate(this->latitude,this->longitude);
}

// deprecated
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
// deprecated
QGeoCoordinate PTZElement::FindEndPoint(double angle,double r){

    return QGeoCoordinate(this->latitude,this->longitude).atDistanceAndAzimuth(r , angle);

    double delta = r/EARTH_RADIUS_IN_METERS;
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
// deprecated
QGeoCoordinate PTZElement::FindEndPointWithRefrece(double angle,double r){

    double delta = r/EARTH_RADIUS_IN_METERS;
    double lat = this->latitude * DEG_TO_RAD;
    double lng = this->longitude * DEG_TO_RAD;
    double th0 =this->angleFromNorth - angle;
    if(th0 < 0) th0 += 360;
    th0 *= DEG_TO_RAD;

    double lat2 = asin(sin(lat)*cos(delta) + cos(lat)*sin(delta)*cos(th0));
    lat2 *= 180/M_PI;

    double lng2 = lng + atan2(sin(th0)*sin(delta)*cos(lat),cos(delta)-sin(lat)*sin(lat2));
    lng2 *= 180/M_PI;
    return QGeoCoordinate(lat2,lng2);

}


