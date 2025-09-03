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

    focus: true              // allows the page to receive key events
    Keys.enabled: true       // enable Keys handler
    Component.onCompleted: {
        forceActiveFocus()   // ensures the Page actually has focus
    }

    FieldView{
        id: fieldViewCpp
        onFieldReady: {
            updateTiles(true);
        }
    }

    property int tileCountX: 20
    property int tileCountY: 20
    property real tileSize: fieldViewCpp.tileSize * 100 // multiply by 100 to get in centimeters

    property real targetYaw: 0
    property real targetPitch: 25
    property real targetRadius: 2000
    property real cameraYaw: 0
    property real cameraPitch: 25
    property real cameraRadius: 2000

    property vector2d targetPosition: getTargetPosition();
    property vector2d position: getTargetPosition();
    property real targetRotation: geolocationService.eulerRotation()
    property real rotation: geolocationService.eulerRotation()

    function getTargetPosition() {
        // Vehicle/world position in centimeters
        const rawPos = fieldViewCpp.coordinateInCentimeters();
        // Field origin in centimeters (world corner)
        const fieldOrigin = fieldViewCpp.fieldOrigin;

        let x = (rawPos.x + fieldOrigin.x);
        let y = (rawPos.y + fieldOrigin.y);

        // Return as Qt.vector2d
        return Qt.vector2d(x, y);
    }


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
        // create tiles with only indices; positioning done by updateTiles()
        for (let x = -tileCountX; x <= tileCountX; x++) {
            for (let y = -tileCountY; y <= tileCountY; y++) {
                tileComponent.createObject(tiles, {
                                               "tileXIndex": x,
                                               "tileYIndex": y,
                                               "logicalX": 0,
                                               "logicalY": 0
                                           });
            }
        }
        // position them once for the current position
        updateTiles(true);
    }

    function updateTiles(force = false) {
        let logicalOriginX = -Math.ceil(position.x / tileSize);
        let logicalOriginY = -Math.ceil(position.y / tileSize);

        for (let t of tiles.children) {
            let logicalX = t.tileXIndex + logicalOriginX;
            let logicalY = t.tileYIndex + logicalOriginY;

            t.x = logicalX * tileSize + tileSize / 2;
            t.z = -logicalY * tileSize - tileSize / 2;

            if (force || t.logicalX !== logicalX || t.logicalY !== logicalY) {

                t.logicalX = logicalX;
                t.logicalY = logicalY;

                let dx = t.tileXIndex;
                let dy = t.tileYIndex;
                let distanceFromCenter = Math.sqrt(dx * dx + dy * dy);

                if (distanceFromCenter > 2)
                    t.resolutionScale = 0.1;
                else
                    t.resolutionScale = 1


                t.materials[0].fieldTex.texture.textureData =
                        fieldViewCpp.getTileFieldTexture(logicalX, logicalY, t.materials[0].fieldTex.texture, t.resolutionScale);

                t.materials[0].hasFieldTexture = t.materials[0].fieldTex.texture.textureData !== null;
            }
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

            position.x = lerpX;
            position.y = lerpY;
            dynamicScene.position.x = position.x;
            dynamicScene.position.z = -position.y;

            rotation = lerpAngle(rotation, targetRotation, 0.15);
            vehicle.eulerRotation.y = -rotation;

            updateTiles();
        }
    }

    Timer{
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            targetPosition = getTargetPosition();
            targetRotation = geolocationService.eulerRotation();
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

    Keys.onReleased: (event) => {
                         // console.log("Key up:", event.key)
                         if (event.key === Qt.Key_W || event.key === Qt.Key_S) {
                             // Stop forward/backward movement
                             fieldViewCpp.setSpeed(0.0)
                         } else if (event.key === Qt.Key_A || event.key === Qt.Key_D) {
                             // Stop rotation
                             fieldViewCpp.setRotationSpeed(0.0)
                         }
                     }

    Component {
        id: tileComponent
        Model{
            id: tileModel
            property int tileXIndex
            property int tileYIndex

            property int logicalX: 0
            property int logicalY: 0

            property real resolutionScale: 0.1

            source: "#Rectangle"
            scale: Qt.vector3d(tileSize / 100, tileSize / 100, tileSize / 100)
            eulerRotation: Qt.vector3d(-90, 0, 0)

            materials: [
                CustomMaterial {
                    property real tileSize: parent.scale.x
                    property real checkerSize: 10
                    property real resolution: 1000 * tileModel.resolutionScale
                    property bool hasFieldTexture: fieldTexture.textureData !== null

                    // property TextureInput sprayedTex: TextureInput{
                    //     texture: Texture{
                    //         id: sprayedTexture
                    //         // textureData: painterService.getTileTexture(tileXIndex, tileYIndex, sprayedTexture)
                    //     }
                    // }

                    property TextureInput fieldTex: TextureInput {
                        texture: Texture {
                            id: fieldTexture
                            textureData: {
                                return fieldViewCpp.getTileFieldTexture(tileXIndex, tileYIndex, fieldTexture, tileModel.resolutionScale)
                            }
                        }
                    }

                    shadingMode: CustomMaterial.Unshaded
                    fragmentShader: "../../assets/shaders/tileFragmentShader.frag"
                    vertexShader: "../../assets/shaders/tileVertexShader.vert"
                }
            ]
        }
    }



    Rectangle{
        anchors.top: parent.top
        anchors.left: parent.left
        width: 500
        // height: 200
        z: 10

        Text{
            id: txt1
            text: "x: " + targetPosition.x + " | y: " + targetPosition.y
            anchors.top: parent.top
        }
        Text{
            id: txt2
            text: "x: " + geolocationService.coordinateInCentimeters().x + " | y: " + geolocationService.coordinateInCentimeters().y
            anchors.top: txt1.bottom
        }
        Text{
            id: txt3
            text: "x: " + fieldViewCpp.fieldOrigin.x + " | y: " + fieldViewCpp.fieldOrigin.y
            anchors.top: txt2.bottom
        }
        Text{
            text: {
                const tileColumn = Math.floor(targetPosition.x / tileSize);
                return tileColumn
            }
            anchors.top: txt3.bottom
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
                    // visible: false
                }
            }

            Node {
                id: dynamicScene
                Node{ id: tiles }

                Model{
                    property vector2d geoPosCm: fieldViewCpp.geoToCentimeters(QtPositioning.coordinate(-21.124493305309855, -48.991681397538315))

                    x: geoPosCm.x - fieldViewCpp.fieldOrigin.x
                    z: -geoPosCm.y + fieldViewCpp.fieldOrigin.y

                    scale: Qt.vector3d(1, 1, 1);
                    y: 50

                    source: "#Cube"
                    materials: PrincipledMaterial{
                        baseColor: "black"
                        roughness: 0.5
                        metalness: 0
                    }
                }

            }

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
                createTiles();
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
