#include "ptzelement.h"

PTZElement::PTZElement(int id)
{
            this->id = id;
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



            root->setTitle("PTZ - " + QString::number(id));
            PTZlabelLat->setText("Latitude : ");
            PTZlabelLong->setText("Longitude :");
            PTZlabelAz->setText("Azimuth Angle :");
            PTZlabelEl->setText("Elevation Angle :");

            root->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);

}
QGroupBox * PTZElement::getRoot(){
    return this->root;
}
void PTZElement::setLat(float lat){
    this->PTZLat->setText(QString::number(lat));

}
void PTZElement::setLong(float lng){
    this->PTZLong->setText(QString::number(lng));

}
void PTZElement::setAzimuthAngle(float angle){
    this->PTZAzimuth->setText(QString::number(angle));

}
void PTZElement::setElevationAngle(float angle){
    this->PTZElevation->setText(QString::number(angle));

}
float PTZElement::getLat(){
    return this->PTZLat->text().toFloat();
}
float PTZElement::getLong(){
    return this->PTZLong->text().toFloat();

}
float PTZElement::getElevationAngle(){
    return this->PTZElevation->text().toFloat();


}
float PTZElement::getAzimuthAngle(){
    return this->PTZAzimuth->text().toFloat();
}
