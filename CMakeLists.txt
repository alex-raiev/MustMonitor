cmake_minimum_required(VERSION 3.16)
project(MustMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Network)
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
pkg_check_modules(MODBUS REQUIRED libmodbus)

qt_add_executable(MustMonitorApp
    src/main.cpp
    src/appconfig.h
    src/appconfig.cpp
    src/modbusclient.h
    src/modbusclient.cpp
    src/dataviewmodel.h
    src/dataviewmodel.cpp
    src/settingsmanager.h
    src/settingsmanager.cpp
)

qt_add_qml_module(MustMonitorApp
    URI MustMonitor
    VERSION 1.0
    QML_FILES 
        Main.qml
        components/Dashboard.qml
        components/Charts.qml
        components/Logs.qml
        components/Settings.qml
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/MustMonitor
)

target_include_directories(MustMonitorApp PRIVATE ${MODBUS_INCLUDE_DIRS})
target_link_directories(MustMonitorApp PRIVATE /usr/local/lib)
target_link_libraries(MustMonitorApp PRIVATE Qt6::Core Qt6::Quick Qt6::Network ${MODBUS_LIBRARIES})
