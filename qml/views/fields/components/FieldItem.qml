import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: fieldItem
    property alias name: nameText.text
    property real area: 0
    property real originLat: 0
    property real originLon: 0
    property url imageSource  // this will hold the passed Image element

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
                    var ha = area / 10000.0
                    var v = ha.toFixed(2)
                    var parts = v.split(".")
                    var intPart = parts[0]
                    var decPart = parts[1]

                    var withSep = ""
                    while (intPart.length > 3) {
                        withSep = "." + intPart.slice(-3) + withSep
                        intPart = intPart.slice(0, intPart.length - 3)
                    }
                    if (intPart.length > 0)
                        withSep = intPart + withSep

                    return "Área: " + withSep + "," + decPart + " ha"
                }
            }

            Text {
                text: {
                    function toDMS(deg, isLat) {
                        var hemisphere = isLat ? (deg >= 0 ? "N" : "S") : (deg >= 0 ? "E" : "W")
                        deg = Math.abs(deg)
                        var d = Math.floor(deg)
                        var m = Math.floor((deg - d) * 60)
                        var s = ((deg - d - m/60) * 3600).toFixed(2)
                        return d + "°" + m + "'" + s + "\" " + hemisphere
                    }

                    "Localização: " + toDMS(originLat, true) + ", " + toDMS(originLon, false)
                }
            }
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
        color: "lightgray"
    }
}
