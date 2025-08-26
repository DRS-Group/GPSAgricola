import QtQuick 2.15
import QtQuick.Effects

Item {
    id: topBarWrapper
    width: parent.width
    height: 64
    z: 1

    property int shadowSize: 5
    property string titleText: "Título"

    // TopBar Rectangle
    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height
        color: "#466905"

        // Back button
        Rectangle {
            id: backButton
            width: parent.height
            height: parent.height
            color: "transparent"

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    stackView.pop()
                }
            }

            Image {
                source: "qrc:/assets/icons/arrow-left.svg"
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                width: parent.width * 0.33
                height: parent.height * 0.33
            }
        }

        // Title
        Text {
            anchors.centerIn: parent
            text: topBarWrapper.titleText
            color: "white"
            font.pixelSize: 20
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }

    // Drop shadow
    MultiEffect {
        anchors.fill: topBar
        source: topBar
        autoPaddingEnabled: true
        shadowEnabled: true
        shadowVerticalOffset: topBarWrapper.shadowSize
        shadowBlur: 1.0
        shadowColor: "#80000000"
    }
}
