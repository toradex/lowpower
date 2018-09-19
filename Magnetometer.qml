import QtQuick 2.4
import QtCharts 2.2

Item {
    property variant designator: ["X °", "Y °", "Z °"]
    property alias xAxis: xAxis
    property alias yAxis: yAxis
    property alias zAxis: zAxis

    ChartView {

        anchors.fill: parent
        antialiasing: true
        backgroundColor: "transparent"
        theme: ChartView.ChartThemeDark

        legend.visible: false

        ValueAxis {
            id: axisX
            min: 0
            max: 10
            tickCount: 0
            gridVisible: false
        }

        ValueAxis {
            id: axisY
            min: -4.0
            max: 4.0
        }

        LineSeries {
            id: xAxis
            axisX: axisX
            axisY: axisY
            color: "#94c840"
        }

        LineSeries {
            id: yAxis
            axisX: axisX
            axisY: axisY
            color: "#065793"
        }

        LineSeries {
            id: zAxis
            axisX: axisX
            axisY: axisY
            color: "#f15f25"
        }
    }
}
