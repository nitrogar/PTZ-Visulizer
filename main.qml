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

        MapItemView{
            model: mapMarker
            delegate: MapQuickItem{
                    coordinate:  model.position
                    sourceItem: Text {
                        z: 1000
                        text: model.number + "--" + model.azimuth


                    }
            }

        }


        /*MapItemView{
            model: mapMarker
            delegate: MapCircle{
                    center: model.position
                    radius: 100
                    border.width: 6
                    color:'red'
            }

        }
*/
        MapItemView{
            model:mapMarker
            delegate: MapCircle{
                radius: model.raduis
                //color: model.circleColor
                center: model.position
                border.width: 6;

            }
        }

        MapItemView{
            model: mapMarker
            delegate: MapPolyline{
                 line.width: 3
                 line.color: 'green'
                 path: [model.position , model.centerLine]
            }

        }

        MapItemView{
            model: mapMarker
            delegate: MapPolyline{
                 line.width: 3
                 line.color: 'red'
                 path: [model.position , model.up]
            }

        }


        MapItemView{
            model: mapMarker
            delegate: MapPolyline{
                 line.width: 3
                 line.color: 'black'
                 path: [model.position , model.viewLine]
            }

        }


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




