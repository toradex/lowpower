import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.VirtualKeyboard 2.2
import QtQuick.Layouts 1.3
import QtQml.RemoteObjects 1.0

import com.toradex.examples.backend 1.0

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

    function updateAxis(axis, nr, fun) {
        axis.clear();
        for (var i = 0; i < storageDepth; i++) {
            var val = fun(nr, i);
            axis.append(storageDepth - i - 1, val.y);
        }
    }

    Connections {
        // Needs to be done this way because we need to address backend
        // created in Cpp
        target: backend
        onAccelerationDataChanged: {
            // if (!initialized) initCharts();
            eventCount++;
            bla.text = "eventCount: " + eventCount;
            updateAxis(accelerometer.xAxis, 0, backend.accelerationData);
            updateAxis(accelerometer.yAxis, 1, backend.accelerationData);
            updateAxis(accelerometer.zAxis, 2, backend.accelerationData);
        }

        onGyroDataChanged: {
            // if (!initialized) initCharts();
            updateAxis(gyroscope.xAxis, 0, backend.gyroData);
            updateAxis(gyroscope.yAxis, 1, backend.gyroData);
            updateAxis(gyroscope.zAxis, 2, backend.gyroData);
        }

        onMagnetoDataChanged: {
            // if (!initialized) initCharts();
            updateAxis(magnetometer.xAxis, 0, backend.magnetoData);
            updateAxis(magnetometer.yAxis, 1, backend.magnetoData);
            updateAxis(magnetometer.zAxis, 2, backend.magnetoData);
        }

        onPowerDataChanged: {
            // if (!initialized) initCharts();
            updateAxis(power.xAxis, 0, backend.powerData);
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
        Label {
            id: bla
            text: "bla"
        }
    }

    function initChart(axis, depth) {
        for (var i = 0; i < depth; i++) {
            axis.append(i, 0);
        }
    }

    function initCharts() {
        try {
            storageDepth = backend.storageDepth();
            bla.text = "start " + storageDepth;
        }
        catch (ex) {
            bla.text = "error " + ex.message;
        }

        initChart(accelerometer.xAxis, storageDepth);
        initChart(accelerometer.yAxis, storageDepth);
        initChart(accelerometer.zAxis, storageDepth);

        initChart(gyroscope.xAxis, storageDepth);
        initChart(gyroscope.yAxis, storageDepth);
        initChart(gyroscope.zAxis, storageDepth);

        initChart(magnetometer.xAxis, storageDepth);
        initChart(magnetometer.yAxis, storageDepth);
        initChart(magnetometer.zAxis, storageDepth);
        initChart(power.xAxis, storageDepth);
        // initialized = true;
    }

    Component.onCompleted: initCharts()

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

