import QtQuick 2.4
import QtCharts 2.2

Item {
    property variant designator: ["X °/s", "Y °/s", "Z °/s"]

    property alias xAxis: xAxis
    property alias yAxis: yAxis
    property alias zAxis: zAxis

    property int depth: 101

    ChartView {

        anchors.fill: parent
        antialiasing: true
        backgroundColor: "transparent"
        theme: ChartView.ChartThemeDark

        legend.visible: false

        ValueAxis {
            id: axisX
            min: 0
            max: depth
            tickCount: 0
            gridVisible: false
        }

        ValueAxis {
            id: axisY
            min: -180.0
            max: 180.0
        }

        LineSeries {
            id: xAxis
            axisX: axisX
            axisY: axisY
        }

        LineSeries {
            id: yAxis
            axisX: axisX
            axisY: axisY
        }

        LineSeries {
            id: zAxis
            axisX: axisX
            axisY: axisY
        }
    }
}
