import QtQuick 2.11
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtCharts 2.0


Window {
    id: ideWindow
    visible: true
    width: defaultWidth + dopwidth
    height: defaultHeight + dopheight
    title: qsTr("QMedicalKit v1.0")

    property int  defaultWidth: 500
    property int  defaultHeight: 240

    property bool err: false
    property int erry: 0

    property int dopheight: 0
    property int dopwidth: 0
    property var plot
    property var map

    property int sp: 5

    signal compile(string sourceCode)
    signal run()



    GeoPopup {
        id: geoPopup
    }

    Connections {
        target: handler
        onShowError: {
            var h = 80;
            var component = Qt.createComponent("ErrorBlock.qml");
            var row = strnum;
            var object = component.createObject(vcontainer, {"height": h, "errtxt": msg, "row": row}, "err")
            dopheight += h + sp;
            err = true
            erry = row - 1
        }
    }

    Connections {
        target: comp
        onShowMsg: {
            var obj = Qt.createQmlObject(parse('
                import QtQuick 2.11
                import QtQuick.Dialogs 1.2
                MessageDialog {
                id: msg
                title: "%s"
                text: "%s"
                icon: msg.title == "Ошибка" ? StandardIcon.Critical : StandardIcon.Information
                onAccepted: {
                    close()
                }
                Component.onCompleted: visible = true
            }', type, what), ideWindow, "msg")
        }
    }

    Connections {
        target: exec
        onPrintVar: {
            var h = 80;
            var component = Qt.createComponent("PrintBlock.qml");
            var count = values.length;
            var vs = varName + " : ";
            for(var i = 0; i < values.length; i++)
                vs += values[i].toString() + " ";
            var object = component.createObject(vcontainer, {"height": h, "head": "Значения", "values": vs, "count": count}, varName)
            dopheight += h + sp;
        }

        onOpenPlot: {
            if(plot) {
                plot.destroy();
                dopwidth -= 400;
            }
            var w = 400;
            var component = Qt.createComponent("PlotBlock.qml");
            console.log("Err:" + component.errorString());
            plot = component.createObject(hcontainer, {"width": inputBox.width}, "plot");
            plot.width = w;
            dopwidth += w;
        }

        onAddVarToPlot: {
            plot.xx.max = values.length;
            var series = plot.createSeries(ChartView.SeriesTypeLine, lineName, plot.xx, plot.yy);
            series.pointsVisible = true;
            series.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
            for(var i = 0; i < values.length; i++) {
                var y = values[i];
                series.append(i, y);
            }
        }
        onOpenTraceMap: {
            if(map) {
                map.destroy();
                dopwidth-=400;
            }
            var w = 400;
            var component = Qt.createComponent("MapBlock.qml");
            map = component.createObject(hcontainer, {"width": w, "head": "Анализ тренировки <i>" + trenName + "</i>" , "foot": " Стабилизация пульса в норме. Давление в норме. ", "body": "Количество сожженных калорий: " + cals + "\n\n" + " Пройденное расстояние в метрах: " + dist}, "map");
            dopwidth += w;
        }
        onAddPointToMap: {
            map.md.append( { 'SYS': SYS, 'DIA': DIA, 'PUL': PUL, 'TEMP': TEMP, 'N': N, 'E': E } );
        }
        onShowMsg: {
            var obj = Qt.createQmlObject(parse('
            import QtQuick 2.11
            import QtQuick.Dialogs 1.2
            MessageDialog {
                title: "%s"
                text: "%s"
                icon: StandardIcon.Critical
                onAccepted: {
                    close()
                }
                Component.onCompleted: visible = true
            }', type, what), ideWindow, "msg");
        }
    }

    Rectangle {
        id: inputBox
        width: parent.width - 20 - dopwidth
        height: parent.height - 60 - dopheight
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10

        color: "white"

        border.color: "lightgrey"
        border.width: 5

        Flickable {
             id: flick
             anchors.margins: 10
             anchors.fill: parent
             contentWidth: edit.paintedWidth
             contentHeight: edit.paintedHeight
             clip: true

             TextEdit {
                 id: edit
                 objectName: "input"
                 width: flick.width
                 focus: true


                 Rectangle {
                         color: "#55FF0000"
                         height: 18
                         width: parent.width + 30
                         y: erry * 18
                         x: parent.x
                         visible: err
                     }
             }
         }
    }

    ButtonGroup {
        buttons: row.children
        onClicked: {
            clearScreen();
            err = false;
            if(button.text == "Compile")
                compile(edit.text);
            if(button.text == "Run")
                run();
            console.log("clicked:", button.text)
        }
    }

    Row {
        id: row
        height: 40
        anchors.right: inputBox.right
        anchors.top: inputBox.bottom
        anchors.topMargin: 5
        anchors.rightMargin: 10

        Button { text: "Run" }
        Button { text: "Compile" }
    }

    Flickable {
         id: flickPrint
         anchors.top: row.bottom
         width: inputBox.width + 10
         height: vcontainer.height > 320 ? 320 : vcontainer.height

         contentWidth: vcontainer.width
         contentHeight: vcontainer.height
         clip: true


         Column {
             id:vcontainer
             padding: 10
             spacing: sp
             width: parent.width - 10
         }

     }

    Row {
        id: hcontainer

        height: ideWindow.height

        anchors.leftMargin: 15
        anchors.left: row.right

    }



    function clearScreen() {
        for(var i = vcontainer.children.length; i > 0 ; i--) {
            console.log("destroying: " + i)
            vcontainer.children[i-1].destroy()
        }
        for(var i = hcontainer.children.length; i > 0 ; i--) {
            console.log("destroying: " + i)
            hcontainer.children[i-1].destroy()
        }
        dopheight = 0;
        dopwidth = 0;
    }

    function parse(str) {
        var args = [].slice.call(arguments, 1),
            i = 0;

        return str.replace(/%s/g, function() {
            return args[i++];
        });
    }
}
