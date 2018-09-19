import QtQuick 2.10
import QtGraphicalEffects 1.0

Item {
    id: indic
    width: 50
    height: width

    function setX(x) {
        indic.x = x - width / 2;
        if (x !== -1000) _refreshVisible()
    }

    function setY(y) {
        indic.y = y - height / 2
        if (x !== -1000) _refreshVisible()
    }

    function showClick() {
        clickAnim.restart()
    }

    function _refreshVisible() {
        opacity = 1;
        hideTimer.restart();
    }

    RadialGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#66FFFFFF" }
            GradientStop { position: 0.35; color: "#aaFFFFFF" }
            GradientStop { position: 0.36; color: "#2fffffff" }
            GradientStop { position: 0.5; color: "transparent" }
            GradientStop { position: 1; color: "transparent" }
        }
        layer.enabled: true
        layer.samples: 8
    }

    Rectangle {
        id: clickOverlay
        width: parent.width
        height: width
        radius: width / 2
        color: "#8fffffff"
        opacity: 0
    }

    ParallelAnimation {
        id: clickAnim

        NumberAnimation {
            target: clickOverlay
            property: "scale"
            duration: 800
            from: 0.3
            to: 1.5
            easing.type: Easing.OutCubic
        }

        NumberAnimation {
            target: clickOverlay
            property: "opacity"
            duration: 800
            from: 1
            to: 0
            easing.type: Easing.OutCubic
        }
    }

    Timer {
        id: hideTimer
        interval: 5000
        onTriggered: hideAnim.restart()
    }

    NumberAnimation on opacity {
        id: hideAnim
        from: 1
        to: 0
        duration: 500
    }
}
