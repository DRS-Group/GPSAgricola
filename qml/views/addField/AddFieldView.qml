import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Effects
import QtQuick.Dialogs
import "../../components"

Page {
    id: fieldsViewPage
    objectName: "AddFieldView"
    title: "AddFieldView"

    property alias field: addField.field

    property Item textInputModalInstance: null

    Connections {
        target: field
        onPolygonChanged: {
            fieldImage.source = field.renderFieldAsUrl(fieldImage.width, fieldImage.height, 5)
        }
    }

    AddFieldView {
        id: addField
    }

    Component{
        id: textInputModalComponent
        TextInputModal{
            value: field.name
            onClose: {
                textInputModalInstance.destroy();
            }
            onPressEnter: {
                if(textInputModalInstance.value === "") return;

                field.name = textInputModalInstance.value;
                textInputModalInstance.destroy();
            }
        }
    }

    function showTextInputModal(){
        if(!textInputModalInstance)
            textInputModalInstance = textInputModalComponent.createObject(fieldsViewPage);
    }

    TopBar{
        id: header
        titleText: "Adicionar Campo"
    }

    Rectangle {
        id: contentArea
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        RowLayout{
            anchors.fill: parent
            spacing: 0

            Rectangle{
                id: inputsWrapper
                color: "white"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                z: 1

                GridLayout{
                    id: formGrid
                    columns: 2
                    columnSpacing: 16
                    rowSpacing: 24
                    anchors.top: parent.top           // must set this
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 48
                    anchors.leftMargin: 24
                    anchors.rightMargin: 24

                    Text{
                        text: "Nome"
                        font.bold: true

                    }
                    Item{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 54

                        Rectangle{
                            id: teste
                            border.color: "#bfbfbf"
                            border.width: 1
                            radius: 8
                            anchors.fill: parent

                            Layout.fillWidth: true
                            Layout.preferredHeight: 50

                            Text{
                                text: field.name || "Sem nome"
                                anchors.centerIn: parent
                                color: {
                                    if(field.name) return "black"
                                    return "#9f9f9f"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    showTextInputModal();
                                }
                            }
                        }

                        MultiEffect {
                            anchors.fill: teste
                            source: teste
                            autoPaddingEnabled: true
                            shadowEnabled: true
                            shadowVerticalOffset: 4
                            shadowBlur: 1.0
                            shadowColor: "#40000000"
                        }
                    }


                    Rectangle {
                        Layout.columnSpan: 2   // span across both columns
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: "#bfbfbf"
                    }


                    Text{
                        text: "Área total"
                        font.bold: true
                    }
                    Item{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 54

                        Rectangle{
                            id: totalArea
                            color: "#dfdfdf"
                            border.color: "#bfbfbf"
                            border.width: 1
                            radius: 8
                            anchors.fill: parent

                            Layout.fillWidth: true
                            Layout.preferredHeight: 50

                            Text{
                                text: {
                                    if(!field.area) return "-";
                                    var ha = field.area / 10000.0;
                                    var v = ha.toFixed(2);
                                    var parts = v.split(".");
                                    var intPart = parts[0];
                                    var decPart = parts[1];

                                    var withSep = "";
                                    while (intPart.length > 3) {
                                        withSep = "." + intPart.slice(-3) + withSep;
                                        intPart = intPart.slice(0, intPart.length - 3);
                                    }
                                    if (intPart.length > 0)
                                        withSep = intPart + withSep;

                                    return withSep + "," + decPart + " ha";
                                }
                                anchors.centerIn: parent
                            }
                        }

                        MultiEffect {
                            anchors.fill: totalArea
                            source: totalArea
                            autoPaddingEnabled: true
                            shadowEnabled: true
                            shadowVerticalOffset: 4
                            shadowBlur: 1.0
                            shadowColor: "#40000000"
                        }
                    }

                    Rectangle {
                        Layout.columnSpan: 2   // span across both columns
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: "#bfbfbf"
                    }

                    Text{
                        text: "Localizacão"
                        font.bold: true
                    }
                    Item{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 54

                        Rectangle{
                            id: coordinates
                            color: "#dfdfdf"
                            border.color: "#c5c5c5"
                            border.width: 1
                            radius: 8
                            anchors.fill: parent

                            Layout.fillWidth: true
                            Layout.preferredHeight: 50

                            Text{
                                text: {
                                    if(field.origin.latitude === 0 && field.origin.longitude === 0) return "-";

                                    function toDMS(deg, isLat) {
                                        var hemisphere = isLat ? (deg >= 0 ? "N" : "S") : (deg >= 0 ? "E" : "W");
                                        deg = Math.abs(deg);
                                        var d = Math.floor(deg);
                                        var m = Math.floor((deg - d) * 60);
                                        var s = ((deg - d - m/60) * 3600).toFixed(2);
                                        return d + "°" + m + "'" + s + "\" " + hemisphere;
                                    }

                                    return toDMS(field.origin.latitude, true) + ", " + toDMS(field.origin.longitude, false);
                                }
                                anchors.centerIn: parent
                            }
                        }

                        MultiEffect {
                            anchors.fill: coordinates
                            source: coordinates
                            autoPaddingEnabled: true
                            shadowEnabled: true
                            shadowVerticalOffset: 4
                            shadowBlur: 1.0
                            shadowColor: "#40000000"
                        }
                    }
                }
            }
            Rectangle{
                id: fieldVisualizationWrapper
                color: "#e0e0e0"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth: 1

                Image{
                    id: fieldImage
                    width: parent.width * 0.75
                    height: width * 0.75
                    fillMode: Image.PreserveAspectFit
                    source: field.renderFieldAsUrl(width, height, 5)
                    anchors.centerIn: parent
                    visible: field.polygon.length >= 3
                }

                Rectangle {
                    id: importWrapper
                    anchors.centerIn: parent
                    width: 300
                    height: 80
                    color: "transparent"
                    border.color: "black"
                    border.width: 1
                    radius: 4
                    visible: field.polygon.length < 3

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: fileDialog.open()
                    }

                    Column {
                        anchors.centerIn: parent
                        spacing: 8

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Clique para importar um arquivo"
                            font.pixelSize: 16
                        }

                        Image {
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "qrc:/assets/icons/tray-arrow-down-black.svg"
                            width: 24
                            height: 24
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    FileDialog {
                        id: fileDialog
                        title: "Selecione o arquivo do talhão"
                        currentFolder: "/home"
                        nameFilters: ["GeoJSON Files (*.geojson)"]
                        onAccepted: {
                            console.log("Selected file:", selectedFile)
                            addField.loadFromFile(selectedFile)
                            // Call your C++ method to import the file here
                            // e.g. field.importPolygonFromFile(file)
                        }
                        onRejected: {
                            console.log("File selection canceled")
                        }
                    }
                }
            }

            MultiEffect {
                anchors.fill: inputsWrapper
                source: inputsWrapper
                autoPaddingEnabled: true
                shadowEnabled: true
                shadowVerticalOffset: 4
                shadowBlur: 1.0
                shadowColor: "#80000000"
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
                source: "qrc:/assets/icons/check.svg"
                height: 24
                width: 24
            }

            MouseArea {
                enabled: field.name !== "" && field.polygon.length > 3

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    if(addField.saveField()){
                        stackView.pop();
                    }
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
            opacity: 0.1
        }
    }
}

/*##^##
Designer {
    D{i:0}D{i:11;cameraSpeed3d:46;cameraSpeed3dMultiplier:1}
}
##^##*/
