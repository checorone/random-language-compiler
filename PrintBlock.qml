import QtQuick 2.0
import QtQuick.Controls 2.4

Rectangle {
    property alias head: headLabel.text
    property alias values: varLabel.text
    property alias count: countLabel.text

    anchors.margins: 10
    width: parent.width

    color: "white"
    border.color: "lightgrey"
    border.width: 5

    Label {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        id: headLabel
        text: "varName"
        color: "black"
    }

    Label {
        anchors.centerIn: parent
        id: varLabel
        text: "var + values"
        color: "black"
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        id: countLabel
        text: "varCount"
        color: "lightgrey"
    }

}
