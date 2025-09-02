import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: jobItem
    property alias name: nameText.text
    property url imageSource  // this will hold the passed Image element
    property int jobType: 0;
    signal click();

    width: ListView.view.width
    height: 120

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20
        Layout.leftMargin: 15
        Layout.rightMargin: 50

        // Image container
        Item {
            width: 100
            height: 100
            Layout.alignment: Qt.AlignVCenter

            Image {
                width: parent.height
                height: parent.width
                source: imageSource
                fillMode: Image.PreserveAspectFit
            }
        }

        // Text info
        Column {
            spacing: 5
            Layout.alignment: Qt.AlignVCenter

            Text {
                id: nameText
                text: "Nome do campo"
                font.bold: true
            }

            Text {
                text: {
                    let strJobType;
                    if(jobType === 0) strJobType="Catação";
                    if(jobType === 0) strJobType="Pulverização";
                    if(jobType === 0) strJobType="Plantação";
                    return "Tipo: " + strJobType;
                }
            }

            // Text {
            //     text: {
            //         function toDMS(deg, isLat) {
            //             var hemisphere = isLat ? (deg >= 0 ? "N" : "S") : (deg >= 0 ? "E" : "W")
            //             deg = Math.abs(deg)
            //             var d = Math.floor(deg)
            //             var m = Math.floor((deg - d) * 60)
            //             var s = ((deg - d - m/60) * 3600).toFixed(2)
            //             return d + "°" + m + "'" + s + "\" " + hemisphere
            //         }

            //         "Localização: " + toDMS(originLat, true) + ", " + toDMS(originLon, false)
            //     }
            // }
        }

        // Spacer to push icon to the right
        Item {
            Layout.fillWidth: true
        }

        // Icon at the end
        Image {
            source: "qrc:/assets/icons/thin-chevron-right-black.svg"
            width: 24
            height: 24
            Layout.alignment: Qt.AlignVCenter
            Layout.rightMargin: 25
        }
    }

    // Bottom line
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 1
        color: "#bfbfbf"
    }

    MouseArea{
        anchors.fill: parent
        cursorShape: "PointingHandCursor"
        onClicked: {
            click();
        }
    }
}
