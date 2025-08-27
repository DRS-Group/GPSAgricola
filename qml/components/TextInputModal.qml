import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

Item {
    id: modal
    anchors.fill: parent
    z: 999
    property string value;
    property string shiftState: "normal"

    signal close()
    signal pressEnter()

    Component.onCompleted: {
        myTextInput.forceActiveFocus();
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
        width: columnLayout.implicitWidth + 32   // optional padding
            height: columnLayout.implicitHeight + 32 // optional padding
        clip: true

        ColumnLayout{
            id: columnLayout
            spacing: 8
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 16
            Item{
                Layout.fillWidth: true
                height: 50
                Rectangle{
                    id: wrapper
                    border.color: "#bfbfbf"
                    border.width: 1
                    radius: 8
                    anchors.fill: parent

                    TextInput{
                        id: myTextInput
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        text: value
                        verticalAlignment: TextInput.AlignVCenter
                        font.pixelSize: 20
                        onTextChanged: {
                            value =  myTextInput.text;
                        }
                        clip: true
                    }
                }

                MultiEffect {
                    anchors.fill: wrapper
                    source: wrapper
                    autoPaddingEnabled: true
                    shadowEnabled: true
                    shadowVerticalOffset: 4
                    shadowBlur: 1.0
                    shadowColor: "#40000000"
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                spacing: 8

                Repeater {
                    model: shiftState === "normal" ? ["q","w","e","r","t","y","u","i","o","p"] : ["Q","W","E","R","T","Y","U","I","O","P"]
                    delegate: Item {
                        width: 60
                        height: 40

                        Rectangle {
                            id: key
                            anchors.fill: parent
                            border.color: "#bfbfbf"
                            border.width: 1
                            radius: 8
                            color: "white"

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.pixelSize: 24
                            }

                            MouseArea {
                                anchors.fill: parent
                                onPressed: myTextInput.text += modelData
                            }
                        }

                        MultiEffect {
                            anchors.fill: key
                            source: key
                            autoPaddingEnabled: true
                            shadowEnabled: true
                            shadowVerticalOffset: 4
                            shadowBlur: 1.0
                            shadowColor: "#40000000"
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                spacing: 8

                Repeater {
                    model: shiftState === "normal" ?  ["a","s","d","f","g","h","j","k","l"] : ["A","S","D","F","G","H","J","K","L"]
                    delegate: Item {
                        width: 60
                        height: 40

                        Rectangle {
                            id: key1
                            anchors.fill: parent
                            border.color: "#bfbfbf"
                            border.width: 1
                            radius: 8
                            color: "white"

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.pixelSize: 24
                            }

                            MouseArea {
                                anchors.fill: parent
                                onPressed: myTextInput.text += modelData
                            }
                        }

                        MultiEffect {
                            anchors.fill: key1
                            source: key1
                            autoPaddingEnabled: true
                            shadowEnabled: true
                            shadowVerticalOffset: 4
                            shadowBlur: 1.0
                            shadowColor: "#40000000"
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                spacing: 8

                Repeater {
                    model: shiftState === "normal" ? ["Shift","z","x","c","v","b","n","m","Backspace"] : ["Shift","Z","X","C","V","B","N","M","Backspace"]
                    delegate: Item {
                        width: {
                            if (modelData === "Shift") return 100;
                            if (modelData === "Backspace") return 100;
                            return 60;
                        }
                        height: 40

                        Rectangle {
                            id: key2
                            anchors.fill: parent
                            border.color: "#bfbfbf"
                            border.width: 1
                            radius: 8
                            color: "white"

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.pixelSize: 24
                                visible: {
                                    if (modelData === "Shift") return false;
                                    if (modelData === "Backspace") return false;
                                    return true;
                                }
                            }

                            Image{
                                anchors.centerIn: parent
                                source: {
                                    if (modelData === "Shift") {
                                        if(shiftState === "normal") return "qrc:/assets/icons/shift-normal-black.svg";
                                        if(shiftState === "shifted") return "qrc:/assets/icons/shift-shifted-black.svg";
                                    }

                                    if (modelData === "Backspace") return "qrc:/assets/icons/backspace-black.svg";

                                    return "";
                                }
                                visible: {
                                    if (modelData === "Shift") return true;
                                    if (modelData === "Backspace") return true;
                                    return false;
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                    if(modelData === "Shift"){
                                        if(shiftState === "normal"){
                                            shiftState = "shifted";
                                        }
                                        else shiftState = "normal";
                                    }
                                    else if (modelData === "Backspace"){
                                        myTextInput.text = myTextInput.text.slice(0, myTextInput.text.length - 1);
                                    }

                                    else {
                                        myTextInput.text += modelData
                                    }
                                }
                            }
                        }

                        MultiEffect {
                            anchors.fill: key2
                            source: key2
                            autoPaddingEnabled: true
                            shadowEnabled: true
                            shadowVerticalOffset: 4
                            shadowBlur: 1.0
                            shadowColor: "#40000000"
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                spacing: 8

                Repeater {
                    model: ["123",",","space",".","Enter"]
                    delegate: Item {
                        width: {
                            if (modelData === "123") return 100;
                            if (modelData === "Enter") return 100;
                            if (modelData === "space") return 380;
                            return 60;
                        }
                        height: 40

                        Rectangle {
                            enabled: {
                                if(modelData === "123") return false;
                                return true;
                            }

                            id: key3
                            anchors.fill: parent
                            border.color: {
                                if(!enabled) return "#20bfbfbf";
                                return "#bfbfbf"
                            }
                            border.width: 1
                            radius: 8
                            color: "#ffffff"

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.pixelSize: {
                                    if(modelData === "space") return 16;
                                    if(modelData === "123") return 16;
                                    return 24
                                }

                                visible: {
                                    if (modelData === "Enter") return false;
                                    return true;
                                }

                                opacity: {
                                    parent.enabled? 1 : 0.5
                                }
                            }
                            Image{
                                anchors.centerIn: parent
                                source: {
                                    if (modelData === "Enter") return "qrc:/assets/icons/keyboard-return-black.svg";
                                    return "";
                                }
                                visible: {
                                    if (modelData === "Enter") return true;
                                    return false;
                                }
                                opacity: {
                                    parent.enabled? 1 : 0.5
                                }
                            }

                            MouseArea {
                                enabled: {
                                    if (modelData === "123") return false;
                                    return true;
                                }

                                anchors.fill: parent
                                onPressed: {
                                    if(modelData === "space") myTextInput.text += " ";
                                    else if (modelData === "Enter") pressEnter();
                                    else
                                        myTextInput.text += modelData;
                                }
                            }
                        }

                        MultiEffect {
                            anchors.fill: key3
                            source: key3
                            autoPaddingEnabled: true
                            shadowEnabled: true
                            shadowVerticalOffset: 4
                            shadowBlur: 1.0
                            shadowColor: "#40000000"
                        }
                    }
                }
            }

        }
        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: false
            z: -1
        }
    }
}
