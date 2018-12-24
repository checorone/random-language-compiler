import QtQuick 2.0
import QtQuick.Controls 2.4

Rectangle {
    property alias errtxt: error.text
    property string row

    anchors.margins: 10
    width: parent.width

    color: "white"
    border.color: "red"
    border.width: 5

    Label {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        id: headLabel
        text: "Ошибка"
        color: "red"
    }

    Label {
        anchors.centerIn: parent
        id: error
        text: "text"
        color: "black"
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        id: countLabel
        text: "Строка: " + row
        color: "red"
    }

}
