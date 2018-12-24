import QtQuick 2.0
import QtQuick.Controls 2.4
import QtLocation 5.9
import QtPositioning 5.8

Rectangle {
    height: parent.height
    property Map map: innerMap
    property ListModel md: pointModel

    property alias head: head.text
    property alias body: body.text
    property alias foot: foot.text

    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
        // specify plugin parameters if necessary
        // PluginParameter {
        //     name:
        //     value:
        // }
    }

    ListModel {
        id: pointModel
    }

    Map {
        id: innerMap
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: parent.height * 0.6
        center: QtPositioning.coordinate(55.67, 37.49) // Moscow
        plugin: mapPlugin
        zoomLevel: 14


        MapItemView
        {
            model: pointModel
            delegate: MapQuickItem {
                id: deleg
                coordinate: QtPositioning.coordinate(N, E)
                anchorPoint: Qt.point(10,10)
                sourceItem: Image {
                    id: image
                    source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        geoPopup.x = ideWindow.dopwidth + deleg.x + geoPopup.width*0.4;
                        geoPopup.y = deleg.y + 5;
                        geoPopup.systxt = SYS;
                        geoPopup.diatxt = DIA;
                        geoPopup.pultxt = PUL;
                        geoPopup.temptxt = TEMP;
                        geoPopup.open();
                    }
                }
            }
        }
    }

    Rectangle {
        border.color: "lightgrey"
        border.width: 3

        anchors.top: innerMap.bottom
        anchors.left: innerMap.left
        width: parent.width
        height: parent.height * 0.4

        Column {
            anchors.margins: 10
            anchors.fill: parent
            Label {
                id: head
                text: "sample"
                anchors.left: parent.left
            }

            Label {
                id: body
                text: "sample"
                anchors.centerIn: parent

            }
            Label {
                id: foot
                text: "sample"
                color: "green"
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }
    }
}
