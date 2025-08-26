import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick3D
import QtQuick3D.Helpers
import QtPositioning
import "../../3DModels"
import "../../assets/shaders"


Page {
    id: fieldViewPage
    objectName: "FieldView"
    title: "FieldView"

    property int tileCountX: 10
    property int tileCountY: 10
    property real tileSize: 100

    property real targetYaw: 0
    property real targetPitch: 25
    property real targetRadius: 2000
    property real cameraYaw: 0
    property real cameraPitch: 25
    property real cameraRadius: 2000

    property vector2d targetPosition: geolocationService.coordinateInCentimeters()
    property vector2d position: geolocationService.coordinateInCentimeters()
    property real targetRotation: geolocationService.eulerRotation()
    property real rotation: geolocationService.eulerRotation()

    function norm360(a) { return (a % 360 + 360) % 360 }
    function norm180(a) { return ((a + 180) % 360 + 360) % 360 - 180 }
    function lerpAngle(a, b, t) {
        let diff = norm180(b - a)
        return norm360(a + diff * t)
    }
    function lerp(from, to, t) {
        return from + (to - from) * t
    }

    function createTiles() {
        for (let x = -tileCountX; x <= tileCountX; x++) {
            for (let y = -tileCountY; y <= tileCountY; y++) {
                tileComponent.createObject(tiles, {
                                               "x": (x * tileSize + tileSize/2) * 100,
                                               "y": 0,
                                               "z": -(y * tileSize + tileSize/2) * 100,
                                               "tileXIndex": x,
                                               "tileYIndex": y
                                           })
            }
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

    Keys.onPressed: (event) => {
                        console.log("Key down:", event.key)
                        if (event.key === Qt.Key_Left) {
                            console.log("Left arrow pressed")
                        }
                    }

    Keys.onReleased: (event) => {
                         console.log("Key up:", event.key)
                         if (event.key === Qt.Key_Left) {
                             console.log("Left arrow released")
                         }
                     }

    Component {
        id: tileComponent
        Model{
            id: tileModel
            property int tileXIndex
            property int tileYIndex

            source: "#Rectangle"
            scale: Qt.vector3d(tileSize, tileSize, tileSize)
            eulerRotation: Qt.vector3d(-90, 0, 0)

            materials: [
                CustomMaterial {
                    property real tileSize: parent.scale.x
                    property real checkerSize: 25
                    property int resolution: 1000

                    property TextureInput sprayedTex: TextureInput{
                        texture: Texture{
                            id: sprayedTexture
                            // textureData: painterService.getTileTexture(tileXIndex, tileYIndex, sprayedTexture)
                        }
                    }

                    property TextureInput fieldTex: TextureInput {
                        texture: Texture {
                            id: fieldTexture
                            textureData: fieldViewCpp.getTileFieldTexture(tileXIndex, tileYIndex, fieldTexture)
                        }
                    }

                    shadingMode: CustomMaterial.Unshaded
                    fragmentShader: "../../assets/shaders/tileFragmentShader.frag"
                    vertexShader: "../../assets/shaders/tileVertexShader.vert"
                }
            ]
        }
    }

    Timer {
        interval: 40; running: true; repeat: true
        onTriggered: {
            cameraYaw   = lerpAngle(cameraYaw, targetYaw - targetRotation, 0.15);
            cameraPitch = cameraPitch + (targetPitch - cameraPitch) * 0.15;
            cameraRadius= cameraRadius + (targetRadius - cameraRadius) * 0.15;
            updateCameraPosition();

            let lerpX = lerp(position.x, targetPosition.x, 0.15);
            let lerpY = lerp(position.y, targetPosition.y, 0.15);

            // const pGeo0 = geolocationService.centimetersToGeo(position);
            // const pGeo1 = geolocationService.centimetersToGeo(Qt.vector2d(lerpX, lerpY));

            // painterService.drawLineGeo(
            //             QtPositioning.coordinate(0, 0),
            //             // QtPositioning.coordinate(-21.12174192463783, -48.96224030991499),
            //             pGeo0,
            //             pGeo1,
            //             141   // gray value
            //             )

            position.x = lerpX;
            position.y = lerpY;
            tiles.position.x = -position.x;
            tiles.position.z = position.y;

            rotation = lerpAngle(rotation, targetRotation, 0.15);
            vehicle.eulerRotation.y = -rotation;
        }
    }

    Timer {
        interval: 40
        running: true
        repeat: true
        onTriggered: {
            for (let t of tiles.children) {
                // painterService.getTileTexture(t.tileXIndex, t.tileYIndex, t);
            }
        }
    }

    Timer{
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            targetPosition = geolocationService.coordinateInCentimeters()
            targetRotation = geolocationService.eulerRotation()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#262626"
        z: -1
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        View3D {
            id: extendedView3D
            width: parent.width
            height: parent.height
            environment: ExtendedSceneEnvironment {
                colorAdjustmentsEnabled: true
                backgroundMode: SceneEnvironment.SkyBox

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

                Tractor_A {
                    id: vehicle
                }
            }

            Node{ id: tiles }

            // Model {
            //     id: groundPlane
            //     source: "#Rectangle"
            //     scale: Qt.vector3d(10000, 10000, 10000)
            //     eulerRotation: Qt.vector3d(-90, 0, 0)
            //     position: Qt.vector3d(0, -20, 0)
            //     materials: PrincipledMaterial{
            //         id: materialTeste
            //         baseColorMap: Texture{
            //             id: texturaTeste
            //             source: "qrc:/assets/ground-grid.jpg"
            //             magFilter: Texture.Nearest
            //             tilingModeHorizontal: Texture.Repeat
            //             scaleV: 25
            //             scaleU: 25
            //         }
            //         roughness: 1
            //         metalness: 0
            //     }
            // }


            // Model{
            //     source: "#Rectangle"
            //     scale: Qt.vector3d(100, 100, 100)
            //     eulerRotation: Qt.vector3d(-90, 0, 0)

            //     materials: [
            //         CustomMaterial {
            //             property real tileSize: parent.scale.x
            //             property real checkerSize: 50
            //             property int resolution: 3

            //             property TextureInput sprayedTex: TextureInput{
            //                 texture: Texture{
            //                     id: teste
            //                     data: painterService.getTileTexture(0, 0, teste)
            //                 }
            //             }

            //             shadingMode: CustomMaterial.Unshaded
            //             fragmentShader: "../../assets/shaders/tileFragmentShader.frag"
            //             vertexShader: "../../assets/shaders/tileVertexShader.vert"
            //         }
            //     ]
            // }


            MouseArea {
                id: cameraMovementMouseArea
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
                             targetRadius = Math.min(Math.max(targetRadius - delta * 5, 100), 50000)
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
                    targetRadius = Math.min(Math.max(targetRadius / scaleDelta, 20), 50000)
                }

                onPinchFinished: {
                    lastScale = 1.0
                }
            }

            Component.onCompleted: {
                updateCameraPosition()
            }
        }
    }
    // Rectangle{
    //     id: rectMenu
    //     color: "#00ff00"
    //     border.width: 0
    //     Layout.alignment: Qt.AlignLeft | Qt.AlignTop
    //     width: parent.width
    //     Layout.preferredHeight: 100
    // }
}

/*##^##
Designer {
    D{i:0}D{i:11;cameraSpeed3d:46;cameraSpeed3dMultiplier:1}
}
##^##*/
