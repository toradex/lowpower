import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.VirtualKeyboard 2.2
import QtQuick.Layouts 1.3
import QtQml.RemoteObjects 1.0

import com.toradex.examples.backend 1.0
//import com.toradex.examples.sensordata 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    property int storageDepth: 0
    property int eventCount: 0

    function mapFromWindow(x, y) {
        return Qt.point(x,y);
    }

    function mapToWindow(x, y) {
        return Qt.point(x,y);
    }

    function updateAxis(axis, sensorData, idx) {
        axis.clear();
        for (var i = 0; i < sensorData.points; i++) {
            var val = sensorData.at(idx, i);
            axis.append(sensorData.points - i - 1, val.y);
        }
    }

    Connections {
        // Needs to be done this way because we need to address backend
        // created in Cpp
        target: backend
        onAccelerationDataChanged: {
            updateAxis(accelerometer.xAxis, sensorData, 0);
            updateAxis(accelerometer.yAxis, sensorData, 1);
            updateAxis(accelerometer.zAxis, sensorData, 2);
        }

        onGyroDataChanged: {
            updateAxis(gyroscope.xAxis, sensorData, 0);
            updateAxis(gyroscope.yAxis, sensorData, 1);
            updateAxis(gyroscope.zAxis, sensorData, 2);
        }

        onMagnetoDataChanged: {
            updateAxis(magnetometer.xAxis, sensorData, 0);
            updateAxis(magnetometer.yAxis, sensorData, 1);
            updateAxis(magnetometer.zAxis, sensorData, 2);
        }

        onPowerDataChanged: {
            updateAxis(power.xAxis, sensorData, 0);
        }
    }

    header: ColumnLayout {
        width: parent.width
        Image {
            source: "images/toradexlogo.png"
            Layout.alignment: Qt.AlignHCenter
        }

        TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            Layout.fillWidth: true

            TabButton {
                text: qsTr("Accelerometer")
            }
            TabButton {
                text: qsTr("Gyroscope")
            }
            TabButton {
                text: qsTr("Magnetometer")
            }
            TabButton {
                text: qsTr("Power Management")
            }

            TabButton {
                text: qsTr("Settings")
            }
        }
    }

    RowLayout {
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        anchors.fill: parent

        ColumnLayout {
            id: test
            Item {
                Layout.fillHeight: true
            }
            SidePanel {
                id: sidepanel
                Layout.fillWidth: true
                Layout.maximumWidth: 100
                Layout.fillHeight: true

                designator: swipeView.currentItem.designator

                sleep.onClicked: backend.sleep();
                power.onClicked: backend.shutdown()
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            SwipeView {
                id: swipeView
                currentIndex: tabBar.currentIndex
                height: parent.height
                width: parent.width
                clip: true

                Accelerometer {
                    id: accelerometer
                }
                Gyroscope {
                    id: gyroscope
                }
                Magnetometer {
                    id: magnetometer
                }
                PowerManagement {
                    id: power
                }
                Settings {
                    id: settings
                }
            }
        }
    }

    RemoteMouseIndicator {
        id: remoteMouse
        x: -1000
        visible: x !== -1000
        z: 99999999999


        Connections {
            target: backend
            onRemotingEnabledChanged: if (!remotingEnabled) remoteMouse.x = -1000
        }

        Connections {
            target: remoteEventHandler
            onRemoteMouseXChanged: remoteMouse.setX(x)
            onRemoteMouseYChanged: remoteMouse.setY(y)
            onRemoteMouseClicked: remoteMouse.showClick()
        }
    }
}

