import QtQuick 2.0
import QtCharts 2.0

ChartView {
    id: chart
    title: "График"

    property ValueAxis xx: xAxis
    property ValueAxis yy: yAxis


    height: parent.height

    ValueAxis{
        id: xAxis
        min: 0.0
        max: 200.0
    }
    ValueAxis{
        id: yAxis
        min: 0.0
        max: 200.0
    }


}
