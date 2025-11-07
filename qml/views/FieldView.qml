import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick3D
import QtQuick3D.Helpers
import QtPositioning
import QtQuick.Effects
import "../../3DModels"
import "../../assets/shaders"
import "../components"
import GPSAgricola 1.0

Page {
    id: fieldViewPage
    objectName: "FieldView"
    title: "FieldView"

    focus: true // allows the page to receive key events
    Keys.enabled: true // enable Keys handler
    Component.onCompleted: {
        forceActiveFocus() // ensures the Page actually has focus
    }

    FieldView {
        id: fieldViewCpp
    }

    property real targetYaw: 0
    property real targetPitch: 25
    property real targetRadius: 2000
    property real cameraYaw: 0
    property real cameraPitch: 25
    property real cameraRadius: 2000

    property vector2d targetPosition: getTargetPosition()
    property vector2d position: getTargetPosition()
    property real targetRotation: geolocationService.eulerRotation()
    property real rotation: geolocationService.eulerRotation()

    function getTargetPosition() {
        // Vehicle/world position in centimeters
        const rawPos = fieldViewCpp.coordinateInCentimeters()
        // Field origin in centimeters (world corner)
        const fieldOrigin = fieldViewCpp.fieldOrigin

        let x = (rawPos.x + fieldOrigin.x)
        let y = (rawPos.y + fieldOrigin.y)

        // Return as Qt.vector2d
        return Qt.vector2d(x, y)
    }

    function norm360(a) {
        return (a % 360 + 360) % 360
    }
    function norm180(a) {
        return ((a + 180) % 360 + 360) % 360 - 180
    }
    function lerpAngle(a, b, t) {
        let diff = norm180(b - a)
        return norm360(a + diff * t)
    }
    function lerp(from, to, t) {
        return from + (to - from) * t
    }

    Timer {
        interval: 40
        running: true
        repeat: true
        onTriggered: {
            cameraYaw = lerpAngle(cameraYaw, targetYaw - targetRotation, 0.15)
            cameraPitch = cameraPitch + (targetPitch - cameraPitch) * 0.15
            cameraRadius = cameraRadius + (targetRadius - cameraRadius) * 0.15
            updateCameraPosition()

            let lerpX = lerp(position.x, targetPosition.x, 0.15)
            let lerpY = lerp(position.y, targetPosition.y, 0.15)

            position.x = lerpX
            position.y = lerpY
            dynamicScene.position.x = position.x
            dynamicScene.position.z = -position.y

            rotation = lerpAngle(rotation, targetRotation, 0.15)
            vehicle.eulerRotation.y = -rotation

            // console.log("ok");
            sprayGeometry.addPathPoint(position.x, position.y)
            sprayGeometry2.addPathPoint(position.x, position.y)
            sprayGeometry3.addPathPoint(position.x, position.y)
        }
    }

    Timer {
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            targetPosition = getTargetPosition()
            targetRotation = geolocationService.eulerRotation()
        }
    }

    function updateCameraPosition() {
        var yawRad = cameraYaw * Math.PI / 180
        var pitchRad = cameraPitch * Math.PI / 180
        var x = cameraRadius * Math.cos(pitchRad) * Math.sin(yawRad)
        var y = cameraRadius * Math.sin(pitchRad)
        var z = cameraRadius * Math.cos(pitchRad) * Math.cos(yawRad)
        var center = Qt.vector3d(0, 100, 0)
        sceneCamera.position = center.plus(Qt.vector3d(x, y, z))
        sceneCamera.lookAt(center)
    }

    Keys.onPressed: event => {
                        // console.log("Key down:", event.key)
                        if (event.key === Qt.Key_W) {
                            // Move forward
                            fieldViewCpp.setSpeed(50)
                        } else if (event.key === Qt.Key_S) {
                            // Move backward / reverse
                            fieldViewCpp.setSpeed(-50)
                        } else if (event.key === Qt.Key_A) {
                            // Rotate left
                            fieldViewCpp.setRotationSpeed(-30.0)
                        } else if (event.key === Qt.Key_D) {
                            // Rotate right
                            fieldViewCpp.setRotationSpeed(30.0)
                        }
                    }

    Keys.onReleased: event => {
                         // console.log("Key up:", event.key)
                         if (event.key === Qt.Key_W || event.key === Qt.Key_S) {
                             // Stop forward/backward movement
                             fieldViewCpp.setSpeed(0.0)
                         } else if (event.key === Qt.Key_A
                                    || event.key === Qt.Key_D) {
                             // Stop rotation
                             fieldViewCpp.setRotationSpeed(0.0)
                         }
                     }

    TopBar {
        id: header
        titleText: ""

        Text {
            color: "white"
            text: "Trabalho 29/09/2025"
            font.weight: 200
            font.pixelSize: 18
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    // Rectangle {
    //     anchors.fill: parent
    //     color: "#262626"
    //     z: -1
    // }
    View3D {
        id: extendedView3D
        width: parent.width
        height: parent.height
        environment: ExtendedSceneEnvironment {
            colorAdjustmentsEnabled: true
            backgroundMode: SceneEnvironment.SkyBox

            lightProbe: Texture {
                source: "qrc:/assets/day-skybox.hdr"
            }
        }

        Node {
            id: scene

            DirectionalLight {
                id: directionalLight
                visible: true
                brightness: 1
                shadowMapQuality: Light.ShadowMapQualityUltra
                castsShadow: true
                eulerRotation: Qt.vector3d(-45, 45, 0)
                position: Qt.vector3d(0, 100, 100)
            }

            PerspectiveCamera {
                id: sceneCamera
                x: -0
                y: 100
                position: Qt.vector3d(0, 100, 100)
                fieldOfView: 80
                z: 689.66284
                clipFar: 100000
                clipNear: 100
            }

            Tractor_A {
                id: vehicle
                //     // visible: false
            }
        }

        Node {
            id: dynamicScene

            Model {
                eulerRotation: Qt.vector3d(90, 0, 0)
                y: 50

                scale: Qt.vector3d(1, 1, 1)
                id: triangleModel
                visible: true
                geometry: SprayGeometry {
                    id: sprayGeometry
                    offset: 0
                }
                materials: [
                    PrincipledMaterial {
                        baseColor: "#00ff00"
                        opacity: 0.25
                        roughness: 1
                    }
                ]
            }
            Model {
                eulerRotation: Qt.vector3d(90, 0, 0)
                y: 50

                scale: Qt.vector3d(1, 1, 1)
                id: triangleModel2
                visible: true
                geometry: SprayGeometry {
                    id: sprayGeometry2
                    offset: 210
                }
                materials: [
                    PrincipledMaterial {
                        baseColor: "#00ff00"
                        opacity: 0.25
                        roughness: 1
                    }
                ]
            }
            Model {
                eulerRotation: Qt.vector3d(90, 0, 0)
                y: 50

                scale: Qt.vector3d(1, 1, 1)
                id: triangleModel3
                visible: true
                geometry: SprayGeometry {
                    id: sprayGeometry3
                    offset: -210
                }
                materials: [
                    PrincipledMaterial {
                        baseColor: "#00ff00"
                        opacity: 0.25
                        roughness: 1
                    }
                ]
            }
            Model {
                id: tileModel

                source: "#Rectangle"
                scale: Qt.vector3d(100000, 100000, 100000)
                eulerRotation: Qt.vector3d(-90, 0, 0)

                materials: [
                    CustomMaterial {
                        property real tileSize: parent.scale.x
                        property real checkerSize: 25
                        property vector2d tileWorldOrigin: mapToGlobal(
                                                               tileModel)

                        shadingMode: CustomMaterial.Unshaded
                        fragmentShader: "../../assets/shaders/tileFragmentShader.frag"
                        vertexShader: "../../assets/shaders/tileVertexShader.vert"
                    }
                ]
            }

            // Model {
            //     property vector2d geoPosCm: fieldViewCpp.geoToCentimeters(
            //                                     QtPositioning.coordinate(
            //                                         -21.124493305309855,
            //                                         -48.991681397538315))

            //     x: 0
            //     z: 0

            //     scale: Qt.vector3d(1, 1, 1)
            //     y: 50

            //     source: "#Cube"
            //     materials: PrincipledMaterial {
            //         baseColor: "black"
            //         roughness: 0.5
            //         metalness: 0
            //     }
            // }
        }

        MouseArea {
            id: cameraMovementMouseArea
            anchors.fill: parent
            drag.target: null
            property real lastX: 0
            property real lastY: 0

            onPressed: mouse => {
                           lastX = mouse.x
                           lastY = mouse.y
                       }

            onPositionChanged: mouse => {
                                   var dx = mouse.x - lastX
                                   var dy = mouse.y - lastY

                                   var sensitivity = 0.5

                                   targetYaw = norm360(targetYaw - dx * 0.5)
                                   targetPitch = Math.min(
                                       Math.max(targetPitch + dy * sensitivity,
                                                -90), 89)

                                   updateCameraPosition()

                                   lastX = mouse.x
                                   lastY = mouse.y
                               }

            onWheel: wheel => {
                         var delta = wheel.angleDelta.y / 10
                         targetRadius = Math.min(Math.max(
                                                     targetRadius - delta * 5,
                                                     100), 50000)
                     }
        }

        PinchArea {
            id: cameraMovementPinchArea
            anchors.fill: parent
            pinch.maximumScale: 3
            pinch.minimumScale: 0.5
            pinch.dragAxis: Pinch.NoDrag

            property real lastScale: 1.0

            onPinchUpdated: {
                var scaleDelta = pinch.scale / lastScale
                lastScale = pinch.scale
                targetRadius = Math.min(Math.max(targetRadius / scaleDelta,
                                                 20), 50000)
            }

            onPinchFinished: {
                lastScale = 1.0
            }
        }

        Component.onCompleted: {
            updateCameraPosition()
        }
    }

    DebugView {
        anchors.top: header.bottom
        source: extendedView3D
        visible: true
        resourceDetailsVisible: true
    }

    // Rectangle{
    //     id: rectMenu
    //     color: "#00ff00"
    //     border.width: 0
    //     Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    //     width: parent.width
    //     Layout.preferredHeight: 100
    // }
    Item {
        id: bottomBarWrapper
        width: parent.width
        height: 64
        z: 1
        anchors.bottom: parent.bottom

        property int shadowSize: 5
        property string titleText: "Título"

        // TopBar Rectangle
        Rectangle {
            id: topBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height
            color: "#466905"

            Row {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8

                // Rectangle 1
                Rectangle {
                    width: (parent.width - 24) / 4
                    height: column1.implicitHeight
                    color: "transparent"
                    anchors.verticalCenter: parent.verticalCenter

                    Column {
                        id: column1
                        spacing: 2
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            color: "white"
                            text: "Latitude"
                            font.weight: 200
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            color: "white"
                            text: "21° 7′ 24.1″ S"
                            font.weight: 600
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

                // Rectangle 2
                Rectangle {
                    width: (parent.width - 24) / 4
                    height: column2.implicitHeight
                    color: "transparent"
                    anchors.verticalCenter: parent.verticalCenter

                    Column {
                        id: column2
                        spacing: 2
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            color: "white"
                            text: "Longitude"
                            font.weight: 200
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            color: "white"
                            text: "48° 59′ 30.6″ O"
                            font.weight: 600
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

                Rectangle {
                    width: (parent.width - 24) / 4
                    height: column3.implicitHeight
                    color: "transparent"
                    anchors.verticalCenter: parent.verticalCenter

                    Column {
                        id: column3
                        spacing: 2
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            color: "white"
                            text: "Progresso"
                            font.weight: 200
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            color: "white"
                            text: "90%"
                            font.weight: 600
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

                Rectangle {
                    width: (parent.width - 24) / 4
                    height: column4.implicitHeight
                    color: "transparent"
                    anchors.verticalCenter: parent.verticalCenter

                    Column {
                        id: column4
                        spacing: 2
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            color: "white"
                            text: "Hora"
                            font.weight: 200
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            color: "white"
                            text: "01:36"
                            font.weight: 600
                            font.pixelSize: 18
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
        }

        // Drop shadow
        MultiEffect {
            anchors.fill: topBar
            source: topBar
            autoPaddingEnabled: true
            shadowEnabled: true
            shadowVerticalOffset: -bottomBarWrapper.shadowSize
            shadowBlur: 1.0
            shadowColor: "#80000000"
        }
    }

    // Item {
    //     id: modal
    //     anchors.fill: parent
    //     z: 999
    //     property string value;
    //     property string shiftState: "normal"

    //     signal close()
    //     signal pressEnter()

    //     // Dark background overlay
    //     Rectangle {
    //         id: background
    //         anchors.fill: parent
    //         color: "black"
    //         opacity: 0.5

    //         MouseArea {
    //             anchors.fill: parent
    //             propagateComposedEvents: false
    //             onClicked: {
    //                 modal.close();
    //             }
    //         }
    //     }

    //     Rectangle {
    //         id: dialog
    //         radius: 16
    //         color: "white"
    //         anchors.centerIn: parent
    //         width: columnLayout.implicitWidth + 64   // optional padding
    //         height: columnLayout.implicitHeight + 64 // optional padding
    //         clip: true

    //         ColumnLayout{
    //             id: columnLayout
    //             spacing: 24
    //             anchors.left: parent.left
    //             anchors.right: parent.right
    //             anchors.verticalCenter: parent.verticalCenter
    //             // anchors.margins: 24

    //             Text{
    //                 text: "Defina qual produto foi pulverizado"
    //                 font.pixelSize: 24
    //                 anchors.horizontalCenter: parent.horizontalCenter
    //                 // anchors.verticalCenter: parent.verticalCenter
    //                 // anchors.margins: 24
    //             }

    //             RowLayout {
    //                 Layout.alignment: Qt.AlignHCenter
    //                 Layout.fillHeight: true
    //                 spacing: 24

    //                 Rectangle{
    //                     height: 128 * 1.5
    //                     width: 128 * 1.5
    //                     radius: 16
    //                     color: "#00aa00"

    //                     Text{
    //                         text: "Produto A"
    //                         font.pixelSize: 24
    //                         anchors.horizontalCenter: parent.horizontalCenter
    //                         anchors.verticalCenter: parent.verticalCenter
    //                         color: "white"
    //                     }
    //                 }

    //                 Rectangle{
    //                     height: 128 * 1.5
    //                     width: 128* 1.5
    //                     radius: 16
    //                     color: "#0055aa"

    //                     Text{
    //                         text: "Produto B"
    //                         font.pixelSize: 24
    //                         anchors.horizontalCenter: parent.horizontalCenter
    //                         anchors.verticalCenter: parent.verticalCenter
    //                         color: "white"
    //                     }
    //                 }
    //             }

    //         }
    //         MouseArea {
    //             anchors.fill: parent
    //             propagateComposedEvents: false
    //             z: -1
    //         }
    //     }
    // }
}

/*##^##
Designer {
    D{i:0}D{i:11;cameraSpeed3d:46;cameraSpeed3dMultiplier:1}
}
##^##*/

