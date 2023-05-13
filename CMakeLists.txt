cmake_minimum_required(VERSION 3.5)

project(Intersection-Traffic-Simulation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        vehicle.cpp
        Simulation.cpp
        Simulation.h
        Vehicle.h
        Button.cpp
        Button.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Intersection-Traffic-Simulation
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Intersection-Traffic-Simulation APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Intersection-Traffic-Simulation SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Intersection-Traffic-Simulation
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set_target_properties(Intersection-Traffic-Simulation PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(Intersection-Traffic-Simulation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Resources:
set(vehicles_resource_files
    "static/images/backgroundNoLights.png"
    "static/images/carRed.jpg"
)

qt_add_resources(Intersection-Traffic-Simulation "vehicles"
    PREFIX
        "/"
    FILES
        ${vehicles_resource_files}
)

install(TARGETS Intersection-Traffic-Simulation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Intersection-Traffic-Simulation)
endif()
