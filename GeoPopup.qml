import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Popup {
    id: popup
    height: 200
    width: 285
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    property alias systxt: systext.text
    property alias diatxt: diatext.text
    property alias pultxt: pultext.text
    property alias temptxt: temptext.text

    padding: 15

    Column {
        anchors.fill: parent

        spacing: 15

        Label {
            text: "Показания:"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            spacing: 5
            Label {
                text: "Систолическое давление: "
            }

            Label {
                id: systext
            }

            Label {
                text: "мм рт.ст."
            }
        }

        Row {
            spacing: 5
            Label {
                text: "Диастолическое давление: "
            }

            Label {
                id: diatext
            }

            Label {
                text: "мм рт.ст."
            }
        }

        Row {
            spacing: 5
            Label {
                text: "Пульс: "
            }

            Label {
                id: pultext
            }

            Label {
                text: "у.в.м."
            }
        }

        Row {
            spacing: 5
            Label {
                text: "Температура: "
            }

            Label {
                id: temptext
            }

            Label {
                text: "°C"
            }

        }
    }


}
