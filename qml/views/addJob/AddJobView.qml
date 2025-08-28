import GpsAgricola 1.0
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Effects
import QtQuick.Dialogs
import "../../components"
import "components"

Page {
    id: addJobViewPage
    objectName: "AddJobView"
    title: "AddJobView"

    property Item fieldSelectorModalInstance: null
    property Item textInputModalInstance: null

    Connections {
        target: viewModel
        function onFieldNameChanged() {
            fieldImage.source = viewModel.renderFieldAsUrl(fieldImage.width, fieldImage.height, 5)
        }
    }

    AddJobView {
        id: addJobView
    }

    AddJobViewModel{
        id: viewModel;
    }

    Component{
        id: fieldSelectorModalComponent
        FieldSelectorModal{
            onSelect: (fieldName) => {
                          console.log("Selected field:", fieldName)
                          viewModel.fieldName = fieldName
                          fieldSelectorModalInstance.destroy();
                      }
            onClose: {
                fieldSelectorModalInstance.destroy();
            }
        }
    }

    function showFieldSelectorModal(){
        if(!fieldSelectorModalInstance)
            fieldSelectorModalInstance = fieldSelectorModalComponent.createObject(addJobViewPage);
    }

    Component{
        id: textInputModalComponent
        TextInputModal{
            value: viewModel.name
            onClose: {
                textInputModalInstance.destroy();
            }
            onPressEnter: {
                if(textInputModalInstance.value === "") return;

                viewModel.name = textInputModalInstance.value;
                textInputModalInstance.destroy();
            }
        }
    }

    function showTextInputModal(){
        if(!textInputModalInstance)
            textInputModalInstance = textInputModalComponent.createObject(addJobViewPage);
    }

    TopBar{
        id: header
        titleText: "Adicionar Trabalho"
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
                    anchors.top: parent.top
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
                                text: viewModel.name || "Sem nome"
                                anchors.centerIn: parent
                                color: {
                                    if(viewModel.name) return "black"
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
                        text: "Campo"
                        font.bold: true
                    }
                    Item{
                        Layout.fillWidth: true
                        Layout.preferredHeight: 54

                        Rectangle{
                            id: field
                            border.color: "#bfbfbf"
                            border.width: 1
                            radius: 8
                            anchors.fill: parent

                            Layout.fillWidth: true
                            Layout.preferredHeight: 50

                            Text{
                                text: viewModel.fieldName || "Sem campo"
                                anchors.centerIn: parent
                                color: {
                                    if(viewModel.fieldName) return "black"
                                    return "#9f9f9f"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    showFieldSelectorModal();
                                }
                            }
                        }

                        MultiEffect {
                            anchors.fill: field
                            source: field
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
                    source: viewModel.renderFieldAsUrl(width, height, 5)
                    anchors.centerIn: parent
                    visible: viewModel.fieldName !== ""
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
                    visible: viewModel.fieldName === ""

                    Column {
                        anchors.centerIn: parent
                        spacing: 8

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Clique para selecionar um campo"
                            font.pixelSize: 16
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            showFieldSelectorModal();
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
                // enabled: field.name !== "" && field.polygon.length > 3

                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    viewModel.fieldName = "Talhão 2";
                    console.log("ok")
                    // if(addField.saveField()){
                    //     stackView.pop();

                    // }
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
