import QtQuick
import QtQuick.Controls

Page {
    title: "MachinesView"

    Rectangle {
        anchors.fill: parent
        color: "#262626" // ou qualquer cor de fundo desejada
        z: -1 // garante que fica atrás dos outros itens
    }

    Label{
        anchors.centerIn: parent
        text: "This is the Machines Screen"
    }

    Button{
        text: "Back"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked: stackView.pop()
    }
}
