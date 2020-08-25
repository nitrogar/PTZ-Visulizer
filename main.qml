import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12
Rectangle{
    id: mapContainer
    color:"#000000"
    anchors.fill:parent
    //Text{
    //    text : mapMarker.current
    //    anchors.centerIn: parent
    //    z : 100
   // }
    Map{
        width: parent.width ;height: parent.height
        id:map
        plugin: mapPlugin
        center: QtPositioning.coordinate(24.449693, 39.631465)
        zoomLevel: 10
        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]

        //MapCircle{
        //    center:QtPositioning.coordinate(24.449693, 39.631465)
        //    color:'black'
        //    radius: 100

        //}
/*
        MapItemView{
            model: mapMarker
            delegate: MapQuickItem{
                    coordinate:  model.position
                    sourceItem: Text {
                        z: 1000
                        text: model.number + "\n" + model.viewLine;
                    }
            }

        }


*/
        MapItemView{
            model: mapMarker
            delegate: PTZCircle{
                    coordinate: model.position
            }

        }
        /*

        MapItemView{
            model: mapMarker
            delegate: PTZCircle{
                    coordinate: model.cp1
            }

        }
        MapItemView{
            model: mapMarker
            delegate: PTZCircle{
                    coordinate: model.cp2
            }

        }
        MapItemView{
            model: mapMarker
            delegate: PTZCircle{
                    coordinate: model.cp3
            }

        }
        MapItemView{
            model: mapMarker
            delegate: PTZCircle{
                    coordinate: model.cp4
            }

        }
        */

        MapItemView{
            model: mapMarker
            delegate: Drone{
                    coordinate: model.cp5
                    z:-100
            }

        }

        MapItemView{
            model:mapMarker
            delegate: MapCircle{
                radius: model.raduis
                //color: model.circleColor
                center: model.position
                border.width: 1;

            }
        }

        MapItemView{
            model: mapMarker
            delegate: MapPolyline{
                 line.width: 3
                 line.color: 'green'
                 path: [model.position , model.droneVector]
            }

        }


        MapItemView{
            model: mapMarker
            delegate: MapPolyline{
                 line.width: 1
                 line.color: '#DC143C'
                 path: [model.position , model.viewLine]
            }

        }

        MapItemView{
            model: mapMarker
            delegate: MapPolygon {
                color: 'red'
                opacity: 0.5
                path: [ model.position , model.leftLine , model.halfLeftLine , model.viewLine , model.halfRightLine ,model.rightLine ]
            }

        }

    /*    MapItemView{
            model:mapMarker
            delegate: Drone{
               coordinate: model.dronePosition
               //radius: 9
              // color : 'white'
               // coordinate: model.dronePosition
            }
        }

*/
    }





    Plugin{
        id:mapPlugin
        name:"osm"
        PluginParameter{
            name:"osm.mapping.custom.host"
            value:"http://localhost:8080/styles/osm-bright/"

        }
        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: true
        }

    }





}




