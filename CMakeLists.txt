cmake_minimum_required(VERSION 3.16)

project(GPSAgricola VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Core
    Quick3D
    Positioning
)

qt_standard_project_setup(REQUIRES 6.8)

set(CMAKE_AUTORCC ON)

qt_add_executable(appGPSAgricola
    main.cpp
    assets.qrc
)

qt_add_qml_module(appGPSAgricola
    URI GPSAgricola
    VERSION 1.0
    RESOURCES
        assets.qrc
        3DModels/meshes/_MergedNode_0_mesh.mesh 3DModels/meshes/plane_001_mesh.mesh
        QML_FILES
        Main.qml
        qml/views/HomeView.qml
        qml/views/FieldView.qml
        qml/views/MachinesView.qml
        qml/components/buttons/MenuButton.qml
        3DModels/Arrow.qml 3DModels/Tractor_A.qml
    SOURCES
        source/application.h source/application.cpp
        source/BaseView.h source/BaseView.cpp
        source/views/HomeView.h source/views/HomeView.cpp
        source/views/FieldView.h source/views/FieldView.cpp
        SOURCES source/BaseService.h source/BaseService.cpp
        SOURCES source/services/GeolocationService.h source/services/GeolocationService.cpp
        SOURCES source/entities/VehicleData.h
        SOURCES source/entities/Tile.h
        SOURCES source/services/PainterService.h source/services/PainterService.cpp
        SOURCES
        SOURCES source/ServicesManager.h source/ServicesManager.cpp
        RESOURCES assets/shaders/tileVertexShader.vert
        RESOURCES
        RESOURCES assets/shaders/tileFragmentShader.frag
        SOURCES source/services/FieldService.h source/services/FieldService.cpp
        SOURCES source/entities/jobs/BaseJob.h source/entities/jobs/BaseJob.cpp
        SOURCES source/entities/Field.h
        SOURCES source/entities/jobs/SprayJob.h source/entities/jobs/SprayJob.cpp
        SOURCES source/services/JobsService.h source/services/JobsService.cpp
        SOURCES source/entities/JobTypes.h
        SOURCES source/entities/TileMap.h
        QML_FILES qml/views/fields/FieldsView.qml
        QML_FILES qml/components/TopBar.qml
        QML_FILES qml/views/fields/components/FieldItem.qml
        SOURCES source/views/FieldsView.h source/views/FieldsView.cpp
        SOURCES source/models/FieldsListModel.h source/models/FieldsListModel.cpp
        QML_FILES qml/views/addField/AddFieldView.qml
        SOURCES source/views/AddFieldView.h source/views/AddFieldView.cpp
        SOURCES source/objects/FieldObject.h
        QML_FILES qml/components/TextInputModal.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appGPSAgricola PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGPSAgricola
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGPSAgricola
    PRIVATE
    Qt6::Quick
    Qt6::Core
    Qt6::Quick3D
    Qt6::Positioning
)

include(GNUInstallDirs)
install(TARGETS appGPSAgricola
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
