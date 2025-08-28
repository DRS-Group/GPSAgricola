import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts
import "../../fields/components"
import GpsAgricola 1.0

Item {
    id: modal
    anchors.fill: parent
    z: 999
    property string value;
    property string shiftState: "normal"

    signal close()
    signal select(string fieldName)

    FieldSelectorCpp{
        id: fieldSelector
    }

    // Dark background overlay
    Rectangle {
        id: background
        anchors.fill: parent
        color: "black"
        opacity: 0.5

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: false
            onClicked: {
                modal.close();
            }
        }
    }

    Rectangle {
        id: dialog
        radius: 16
        color: "white"
        anchors.centerIn: parent
        width: 780
        height: 500
        clip: true

        Item {
            anchors.fill: parent
            anchors.margins: 16   // padding inside the dialog

            ListView {
                id: listView
                anchors.fill: parent
                model: fieldSelector.fields
                spacing: 8  // optional spacing between items

                delegate: FieldItem {
                    width: listView.width    // ✅ make each FieldItem stretch horizontally
                    name: model.name
                    area: model.area
                    originLat: model.origin.latitude
                    originLon: model.origin.longitude
                    imageSource: fieldSelector.renderFieldAsBase64(model.name, 100, 100, 2)

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            modal.select(model.name)
                        }
                    }
                }
            }
        }
    }

}
