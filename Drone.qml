import QtQuick 2.12
import QtLocation 5.12
import QtPositioning 5.12

MapQuickItem{

    //property alias lableText: lable.text
    id: circle
    anchorPoint.x: circle.width/2
    anchorPoint.y: circle.height/2
    sourceItem:
        Image{
            source: "../PTZVisulizer/drone.jpg"
            sourceSize.width: 10
            sourceSize.height: 10
        }






}
