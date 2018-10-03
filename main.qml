import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.VirtualKeyboard 2.2
import QtQuick.Layouts 1.3
import QtQml.RemoteObjects 1.0
import QtCharts 2.2

import com.toradex.examples.backend 1.0
//import com.toradex.examples.sensordata 1.0

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("Tabs")
    overlay.parent: root

    property int storageDepth: 11
    property int eventCount: 0
    property bool accInitialized: false;
    property bool magInitialized: false;
    property bool gyroInitialized: false;
    property bool powerInitialized: false;
    property int i;
    property var val;

    property bool openGLenabled: processSettings.webgl;

    function updateAxis(axis, sensorData, idx) {
        storageDepth = sensorData.points - 1;
        axis.clear();
        for (i = 0; i < sensorData.points - 1; i++) {
            val = sensorData.at(idx, sensorData.points - i - 1);
            axis.append(i, val);
        }
    }


    Rectangle {
        id: root
        color: window.color
        width: 800
        height: 480
        anchors.centerIn: parent
        // clip: true

        function mapFromWindow(x, y) {
            return window.contentItem.mapToItem(root, x, y);
        }

        function mapToWindow(x, y) {
            return window.contentItem.mapFromItem(root, x, y);
        }

        Timer {
            id: refreshTimer
            interval: 500
            running: true
            repeat: true
            onTriggered: {
                switch (swipeView.currentIndex) {
                case 0:
//                    storageDepth = backend.accelerationData.points - 1;
//                    backend.updateAcceleration(0, accelerometer.xAxis);
//                    backend.updateAcceleration(1, accelerometer.yAxis);
//                    backend.updateAcceleration(2, accelerometer.zAxis);
                    updateAxis(accelerometer.xAxis, backend.accelerationData, 0);
                    updateAxis(accelerometer.yAxis, backend.accelerationData, 1);
                    updateAxis(accelerometer.zAxis, backend.accelerationData, 2);
                    break;
                case 1:
                    updateAxis(gyroscope.xAxis, backend.gyroData, 0);
                    updateAxis(gyroscope.yAxis, backend.gyroData, 1);
                    updateAxis(gyroscope.zAxis, backend.gyroData, 2);
                    break;
                case 2:
                    updateAxis(magnetometer.xAxis, backend.magnetoData, 0);
                    updateAxis(magnetometer.yAxis, backend.magnetoData, 1);
                    updateAxis(magnetometer.zAxis, backend.magnetoData, 2);
                    break;
                case 3:
                    updateAxis(power.xAxis, backend.powerData, 0);
                    break;

                }
            }
        }
//        RemoteMouseIndicator {
//            id: remoteMouse
//            x: -1000
//            visible: x !== -1000
//            z: 99999999999

//            Connections {
//                target: backend
//                onRemotingEnabledChanged: if (!remotingEnabled) remoteMouse.x = -1000
//            }

//            Connections {
//                target: remoteEventHandler
//                onRemoteMouseXChanged: if (backend.remoteControlEnabled) remoteMouse.setX(x)
//                onRemoteMouseYChanged: if (backend.remoteControlEnabled) remoteMouse.setY(y)
//                onRemoteMouseClicked: if (backend.remoteControlEnabled) remoteMouse.showClick()
//            }
//        }

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent

            ColumnLayout {
                id: header
                width: parent.width
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
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
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                Layout.fillWidth: true
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
                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onEntered: {openGLenabled = false;}
                        onExited: {openGLenabled = processSettings.webgl;}
                        SwipeView {
                            id: swipeView
                            currentIndex: tabBar.currentIndex
                            height: parent.height
                            width: parent.width
                            clip: true



                            Accelerometer {
                                id: accelerometer
                                depth: storageDepth - 1
                                openGL: openGLenabled
                            }
                            Gyroscope {
                                id: gyroscope
                                depth: storageDepth - 1
                                openGL: openGLenabled
                            }
                            Magnetometer {
                                id: magnetometer
                                depth: storageDepth - 1
                                openGL: openGLenabled
                            }
                            PowerManagement {
                                id: power
                                depth: storageDepth - 1
                                openGL: openGLenabled
                            }
                            Settings {
                                id: settings
                            }
                            onCurrentIndexChanged: console.log("Active index " + currentIndex);
                        }
                    }
                }
            }
        }
    }
}
