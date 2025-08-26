import QtQuick
import QtQuick.Controls
import QtQuick3D.Helpers

ApplicationWindow {
    visible: true
    width: 1280
    height: 800
    title: "GPS Agricola"
    color: "#262626"

    StackView {
        id: stackView
        initialItem: "qml/views/HomeView.qml"
        anchors.fill: parent

        onCurrentItemChanged: {
            application.onStackViewItemChanged(currentItem);
        }

        pushEnter: Transition {
            PropertyAnimation {
                property: "y"
                from: stackView.height
                to: 0
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "y"
                from: 0
                to: -stackView.height
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "y"
                from: -stackView.height
                to: 0
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "y"
                from: 0
                to: stackView.height
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
    }
}
