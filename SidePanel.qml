import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    property variant designator: ["X", "Y", "Z"]

    property alias power: power
    property alias sleep: sleep

    function changeDesignator(designator) {
        if (!designator)
            return;
        designator_x.text = designator[0];
        designator_y.text = designator[1];
        designator_z.text = designator[2];
    }

    onDesignatorChanged: changeDesignator(designator)

    id: sidepanel
    width: 100
    height: 400

    Item {
        id: poweritems
        x: 0
        y: 180
        width: 100
        height: 220
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Button {
            id: sleep
            x: 0
            y: 6
            width: 100
            height: 100
            text: qsTr("Button")
            display: AbstractButton.IconOnly
            Image {
                opacity: 1
                anchors.rightMargin: 2
                anchors.leftMargin: 2
                anchors.bottomMargin: 2
                anchors.topMargin: 2
                anchors.fill: parent
                source: "images/sleep.png"
                fillMode: Image.Tile
            }
        }

        Button {
            id: power
            x: 0
            y: 120
            width: 100
            height: 100
            text: qsTr("Button")
            display: AbstractButton.IconOnly
            Image {
                opacity: 1
                anchors.rightMargin: 2
                anchors.leftMargin: 2
                anchors.bottomMargin: 2
                anchors.topMargin: 2
                anchors.fill: parent

                source: "images/shutdown.png"
            }
        }
    }

    Item {
        id: designators
        x: 0
        y: 0
        width: 100
        height: 180

        Text {
            id: designator_y
            x: 6
            y: 63
            color: "#065793"
            text: qsTr("Y m/s²")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 25
            font.bold: true
        }

        Text {
            id: designator_z
            x: 6
            y: 115
            color: "#f15f25"
            text: qsTr("Z m/s²")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 25
            font.bold: true
        }

        Text {
            id: designator_x
            x: 6
            y: 10
            color: "#94c840"
            text: qsTr("X m/s²")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pixelSize: 25
        }
    }
}
