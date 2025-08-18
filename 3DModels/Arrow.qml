import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources

    // Nodes:
    Model {
        id: plane
        objectName: "Plane"
        source: "meshes/plane_001_mesh.mesh"
        materials: [
            main_Color_material
        ]
    }

    Node {
        id: __materialLibrary__

        PrincipledMaterial {
            id: main_Color_material
            objectName: "Main Color"
            baseColor: "#ff04560a"
            roughness: 0.5
            cullMode: PrincipledMaterial.NoCulling
            alphaMode: PrincipledMaterial.Opaque
        }
    }

    // Animations:
}
