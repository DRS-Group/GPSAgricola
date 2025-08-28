import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Effects
import "../../components"
import "components"

Page {
    id: jobsViewPage
    objectName: "JobsView"
    title: "JobsView"

    JobsView {
        id: jobsView
    }

    TopBar{
        id: header
        titleText: "Trabalhos"
    }

    Rectangle {
        id: contentArea
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "white"

        ListView {
            anchors.fill: parent
            model: jobsView.jobs

            delegate: JobItem {
                name: model.name
                imageSource: jobsView.renderFieldAsBase64(model.fieldName, 100, 100, 2)
            }
        }

        Rectangle {
            id: addButton
            color: "#466905"
            radius: width / 2;
            width: 64
            height: width

            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 16   // offset from edges

            Image {
                anchors.centerIn: parent
                source: "qrc:/assets/icons/plus.svg"
                height: 24
                width: 24
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    stackView.push("../addJob/AddJobView.qml")
                }
            }
        }
        MultiEffect {
            anchors.fill: addButton
            source: addButton
            autoPaddingEnabled: true
            shadowEnabled: true
            shadowVerticalOffset: 4
            shadowBlur: 1.0
            shadowColor: "#80000000"
        }
    }
}

/*##^##
Designer {
    D{i:0}D{i:11;cameraSpeed3d:46;cameraSpeed3dMultiplier:1}
}
##^##*/
