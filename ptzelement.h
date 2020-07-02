#ifndef PTZELEMENT_H
#define PTZELEMENT_H
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class PTZElement
{
public:
    PTZElement(int id);
    void setLat(float lat);
    void setLong(float lng);
    void setAzimuthAngle(float angle);
    void setElevationAngle(float angle);
    float getLat();
    float getLong();
    float getElevationAngle();
    float getAzimuthAngle();
    QGroupBox * getRoot();

private:
        int  id;
       QGroupBox *root;
       QVBoxLayout *verticalLayout_9;
       QHBoxLayout *horizontalLayout;
       QLabel *PTZlabelLat;
       QLineEdit *PTZLat;
       QHBoxLayout *horizontalLayout_18;
       QLabel *PTZlabelLong;
       QLineEdit *PTZLong;
       QHBoxLayout *horizontalLayout_19;
       QLabel *PTZlabelAz;
       QLineEdit *PTZAzimuth;
       QHBoxLayout *horizontalLayout_9;
       QLabel *PTZlabelEl;
       QLineEdit *PTZElevation;
};

#endif // PTZELEMENT_H
