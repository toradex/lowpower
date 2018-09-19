import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Item {

    Switch {
        id: sw
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -90
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Enable Remoting"
        checked: backend.remotingEnabled
        onToggled: {
            backend.remotingEnabled = checked
        }
        onCheckedChanged: {
            if (checked)
                remoteEventHandler.startHandlingEvents(root);
            else
                remoteEventHandler.stopHandlingEvents();
        }
    }

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: sw.bottom
        anchors.topMargin: 48
        spacing: 16
        visible: backend.remotingEnabled

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
            text: "You can now connect to the following address from a Web browser: "
            color: "#C4C4C0"
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 24
            text: "http://" + backend.serverAddress + ":8080"
            font.bold: true
        }
    }
}
