import QtQuick
import QtQuick3D

Node {
    id: node0

    scale: Qt.vector3d(100, 100, 100)

    // Resources

    // Nodes:
    Model {
        id: _MergedNode_0
        objectName: "$MergedNode_0"
        source: "meshes/_MergedNode_0_mesh.mesh"
        materials: [
            black_metal_material,
            glass_material,
            giroflex_lights_material,
            wheel_material,
            front_light_material,
            main_Color_material7,
            rear_lights_material,
            tire_material,
            plane_001_mesh,
            mirror_material
        ]
        castsShadows: true
        receivesShadows: true
    }

    Node {
        id: __materialLibrary__

        PrincipledMaterial {
            id: black_metal_material
            objectName: "black metal"
            baseColor: "#ff080808"
            metalness: 0.19999998807907104
            roughness: 0.6363636255264282
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: glass_material
            objectName: "glass"
            baseColor: "#ff060606"
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: giroflex_lights_material
            objectName: "giroflex lights"
            baseColor: "#ffffff00"
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: wheel_material
            objectName: "wheel"
            baseColor: "#ffb3cc04"
            metalness: 0.23181821405887604
            roughness: 0.20454543828964233
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: front_light_material
            objectName: "front light"
            baseColor: "#ffcccc5b"
            roughness: 0.47727271914482117
            emissiveFactor: Qt.vector3d(1, 0.999788, 0.445893)
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: main_Color_material7
            objectName: "Main Color"
            baseColor: "#1c9922"
            metalness: 0.3
            roughness: 0.1
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: rear_lights_material
            objectName: "rear lights"
            baseColor: "#ffff0000"
            emissiveFactor: Qt.vector3d(0.5, 0, 0)
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: tire_material
            objectName: "tire"
            baseColor: "#ff010101"
            roughness: 0.5
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: plane_001_mesh
            objectName: "black plastic"
            baseColor: "#ff030303"
            metalness: 0.13636362552642822
            roughness: 0.7909091114997864
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }

        PrincipledMaterial {
            id: mirror_material
            objectName: "mirror"
            metalness: 1
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }
    }

    // Animations:
}

/*##^##
Designer {
    D{i:0;cameraSpeed3d:16;cameraSpeed3dMultiplier:1}
}
##^##*/
