import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects
import "../components/buttons"
import QtMultimedia

Page {
    id: page
    title: "HomeView"
    objectName: "HomeView"
    Rectangle {
        anchors.fill: parent
        color: "#262626" // ou qualquer cor de fundo desejada
        z: -3 // garante que fica atrás dos outros itens
    }

    Video {
        id: backgroundVideo
        anchors.fill: parent
        source: "qrc:/assets/background-video.mp4"
        autoPlay: true
        loops: MediaPlayer.Infinite
        fillMode: VideoOutput.PreserveAspectCrop
        z: -2
        opacity: 0.5
    }

    // ShaderEffectSource {
    //     id: videoSource
    //     sourceItem: backgroundVideo
    //     anchors.fill: parent
    //     live: true
    //     visible: false
    //     z: -1
    // }

    // // Apply blur to the video
    // FastBlur {
    //     id: blurredVideo
    //     anchors.fill: parent
    //     source: videoSource
    //     radius: 20 // adjust blur strength here
    // }

    // // Semi-transparent black overlay on top of the blurred video
    // Rectangle {
    //     id: blackOverlay
    //     anchors.fill: parent
    //     color: "black"
    //     opacity: 0.5
    //     z: 0
    // }

    RowLayout {
        id: rowLayout
        width: implicitWidth
        height: implicitHeight
        anchors.horizontalCenterOffset: 0
        anchors.centerIn: parent
        spacing: Window.width * 0.05

        MenuButton {
            id: menuButton
            imageSource: "qrc:/assets/jobs.jpg"
            onClicked: {
                stackView.push("jobs/JobsView.qml")
            }
            text: "Trabalhos"
        }

        MenuButton {
            id: menuButton1
            imageSource: "qrc:/assets/fields.jpg"
            onClicked: {
                stackView.push("fields/FieldsView.qml")
            }

            text: "Campos"
        }

        MenuButton {
            id: menuButton2
            imageSource: "qrc:/assets/machines.jpg"
            onClicked: {
                stackView.push("MachinesView.qml")
            }
            text: "Maquinário"
        }
    }
}
