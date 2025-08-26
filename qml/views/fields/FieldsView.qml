import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Effects
import "../../components"
import "components"

Page {
    id: fieldsViewPage
    objectName: "FieldsView"
    title: "FieldsView"

    TopBar{
        id: header
        titleText: "Campos"
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
            model: fieldModel

            delegate: FieldItem {
                name: model.name
                area: model.area
                originLat: model.originLat
                originLon: model.originLon
                imageSource: fieldsViewCpp.renderFieldAsBase64(model.name, 100, 100, 2)
            }
        }
    }
}

/*##^##
Designer {
    D{i:0}D{i:11;cameraSpeed3d:46;cameraSpeed3dMultiplier:1}
}
##^##*/
