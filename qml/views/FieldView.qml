import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects
import QtQuick3D
import QtQuick3D.Helpers
import "../../3DModels"

Page {
    property real targetYaw: 0
    property real targetPitch: 25
    property real targetRadius: 2000

    property real cameraYaw: 0
    property real cameraPitch: 25
    property real cameraRadius: 2000

    function norm360(a) { return (a % 360 + 360) % 360 }         // [0,360)
    function norm180(a) { return ((a + 180) % 360 + 360) % 360 - 180 } // (-180,180]

    function lerpAngle(a, b, t) {
        // delta mais curto entre a e b
        let diff = norm180(b - a);
        return norm360(a + diff * t);
    }

    Timer {
        interval: 16
        running: true
        repeat: true
        onTriggered: {
            let lerp = (a, b, t) => a + (b - a) * t
            let smooth = 0.15

            cameraYaw   = lerpAngle(cameraYaw,   targetYaw,   0.15)
            cameraPitch = cameraPitch + (targetPitch - cameraPitch) * 0.15
            cameraRadius= cameraRadius + (targetRadius - cameraRadius) * 0.15

            updateCameraPosition()
        }
    }


    function updateCameraPosition() {
        var yawRad = cameraYaw * Math.PI / 180
        var pitchRad = cameraPitch * Math.PI / 180

        var x = cameraRadius * Math.cos(pitchRad) * Math.sin(yawRad)
        var y = cameraRadius * Math.sin(pitchRad)
        var z = cameraRadius * Math.cos(pitchRad) * Math.cos(yawRad)

        // Ponto de rotação
        var center = Qt.vector3d(0, 100, 0)

        // Nova posição é em torno do ponto central
        sceneCamera.position = center.plus(Qt.vector3d(x, y, z))

        // Sempre olhar para o ponto central
        sceneCamera.lookAt(center)
    }


    title: "FieldView"


    Button {
        anchors.right: parent.right
        text: "Toggle DebugView"
        onClicked: debugView.visible = !debugView.visible
        DebugView {
            id: debugView
            source: extendedView3D
            visible: false
            anchors.top: parent.bottom
            anchors.right: parent.right
            resourceDetailsVisible: true
        }
        z: 1000
    }
    Rectangle {
        anchors.fill: parent
        color: "#262626" // ou qualquer cor de fundo desejada
        z: -1 // garante que fica atrás dos outros itens
    }

    ColumnLayout{
        id: rowLayout
        width: parent.width
        height: parent.height
        spacing: 0

        Rectangle{
            color: "#ff0000"
            radius: 0
            border.width: 0
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            width: parent.width
            Layout.fillHeight: true
            Layout.preferredHeight: 0

            View3D {
                id: extendedView3D
                width: parent.width
                height: parent.height
                environment: sceneEnvironment

                ExtendedSceneEnvironment {
                    id: sceneEnvironment
                    colorAdjustmentsEnabled: true
                    backgroundMode: SceneEnvironment.SkyBox
                    antialiasingQuality: SceneEnvironment.High
                    antialiasingMode: SceneEnvironment.MSAA

                    lightProbe: Texture{
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
                        clipFar: 1000000
                    }
                }

                Node{
                    id: node
                    Tractor_A{
                    }

                    Model {
                        id: groundPlane
                        source: "#Rectangle"
                        eulerRotation: Qt.vector3d(-90,-1,0)
                        scale: Qt.vector3d(10000, 10000, 1)
                        materials: whiteMaterial
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    drag.target: null
                    property real lastX: 0
                    property real lastY: 0

                    onPressed: (mouse)=> {
                                   lastX = mouse.x
                                   lastY = mouse.y
                               }

                    onPositionChanged:(mouse) => {
                                          var dx = mouse.x - lastX
                                          var dy = mouse.y - lastY


                                          var sensitivity = 0.5

                                          targetYaw = norm360(targetYaw - dx * 0.5)
                                          targetPitch = Math.min(Math.max(targetPitch + dy * sensitivity, 0), 89)

                                          updateCameraPosition()

                                          lastX = mouse.x
                                          lastY = mouse.y
                                      }

                    onWheel: (wheel) => {
                                 var delta = wheel.angleDelta.y / 10
                                 targetRadius = Math.min(Math.max(targetRadius - delta * 5, 100), 5000)
                             }

                }

                PinchArea {
                    anchors.fill: parent
                    pinch.maximumScale: 3
                    pinch.minimumScale: 0.5
                    pinch.dragAxis: Pinch.NoDrag

                    property real lastScale: 1.0

                    onPinchUpdated: {
                        var scaleDelta = pinch.scale / lastScale
                        lastScale = pinch.scale
                        targetRadius = Math.min(Math.max(targetRadius / scaleDelta, 20), 5000)
                    }


                    onPinchFinished: {
                        lastScale = 1.0
                    }
                }


                Component.onCompleted: updateCameraPosition()

            }
        }
        Rectangle{
            color: "#00ff00"
            border.width: 0
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            width: parent.width
            Layout.preferredHeight: 100
        }
    }

    Item {
        id: __materialLibrary__

        PrincipledMaterial {
            id: whiteMaterial
            roughness: 1
            baseColor: "white"
        }
    }
}





/*##^##
Designer {
    D{i:0}D{i:7;cameraSpeed3d:16;cameraSpeed3dMultiplier:1}D{i:10;cameraSpeed3d:16;cameraSpeed3dMultiplier:1}
}
##^##*/
