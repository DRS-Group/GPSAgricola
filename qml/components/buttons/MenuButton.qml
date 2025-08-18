import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Effects

Item {
    id: root
    property alias text: label.text
    property alias imageSource: image.source
    signal clicked

    Layout.alignment: Qt.AlignCenter

    width: Window.width * 0.25
    height: Window.height * 0.55

    DropShadow {
        anchors.fill: backgroundRect
        horizontalOffset: 0
        verticalOffset: 4
        radius: 12
        samples: 16
        color: "#80000000"
        source: backgroundRect
    }

    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        radius: 16
        clip: true
        smooth: true
        antialiasing: true

        Image {
            id: image
            anchors.fill: parent
            source: root.imageSource
            fillMode: Image.PreserveAspectCrop
            width: parent.width
            height: parent.height

            layer.enabled: true
            layer.smooth: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: backgroundRect.width
                    height: backgroundRect.height
                    radius: backgroundRect.radius
                    color: "white"
                }
            }
        }

        Label {
            id: label
            text: qsTr("Jobs")
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: parent.height * 0.05
            font.weight: Font.Bold
            font.pixelSize: 16 * 1.25
            color: "#ffffff"
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                console.log("clicked")
                root.clicked()
            }
        }
    }
}
