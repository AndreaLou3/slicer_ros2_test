cmake_minimum_required(VERSION 3.13.4)

project(ROSPointSubscriberExtension)

find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

# Find slicer_ros2_module
find_package(slicer_ros2_module REQUIRED)

add_subdirectory(Logic)

set(MODULE_EXPORT_DIRECTIVE "Q_SLICER_QTMODULE_ROSPOINTSUBSCRIBER_EXPORT")

set(qt_module_SRCS
    qSlicerROSPointSubscriberModule.cxx
)

set(qt_module_MOC_SRCS
    qSlicerROSPointSubscriberModule.h
)

slicerMacroBuildQtModule(
    NAME ROSPointSubscriber
    SRCS ${qt_module_SRCS}
    MOC_SRCS ${qt_module_MOC_SRCS}
    TARGET_LIBRARIES
        Logic
        slicer_ros2_module
)

