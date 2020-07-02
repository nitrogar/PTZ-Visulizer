import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12
Rectangle{
    id : root
    width:  ; height:700
    color: "#00a3fc"
    anchors.fill: parent
/*
    Rectangle{
        id: sideBar
        color: "#66aa66"
        width: root.width *1/4
        anchors {
            bottom: root.bottom
            top: root.top
            left: root.left

        }

    }
*/
    Rectangle{
        id: mapContainer
        color:"#000000"
        //width: root.width //*3/4
        anchors {
            //right: root.right
            //left : sideBar.right
            //top: root.top
           // bottom: root.bottom
           // leftMargin: 4
            fill:parent
        }

        Map{
         width: parent.width ;height: parent.height
         id:map
         plugin: mapPlugin
         center: QtPositioning.coordinate(24.4765,46.7949)
         zoomLevel: 10
         activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
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


}

/*


    */
