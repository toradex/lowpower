import QtQuick 2.4
import QtCharts 2.2

Item {
    property variant designator: ["mW", "", ""]
    property alias xAxis: xAxis

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
            min: 0
            max: 600
        }

        LineSeries {
            id: xAxis
            axisX: axisX
            axisY: axisY
        }
    }
}
