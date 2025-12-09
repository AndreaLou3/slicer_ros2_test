project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE
    "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

# -----------------------------------------------------------------------------
# Paths to your generated ROS2 MRML/Logic node code
set(ROS2_MODULE_SOURCE_DIR "/home/andrealou3/ros2_ws/src/slicer_ros2_module")
set(ROS2_MODULE_BUILD_DIR  "/home/andrealou3/ros2_ws/src/slicer_ros2_module/build")

# -----------------------------------------------------------------------------
# ROS 2 dependencies (modern, correct)
find_package(rclcpp REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(service_msgs REQUIRED)
find_package(rcpputils REQUIRED)

# -----------------------------------------------------------------------------
# Include dirs for Slicer + your generated ROS2 MRML/Logic headers
set(${KIT}_INCLUDE_DIRECTORIES
  ${ROS2_MODULE_SOURCE_DIR}/MRML
  ${ROS2_MODULE_SOURCE_DIR}/Logic
  ${ROS2_MODULE_BUILD_DIR}/MRML
  ${ROS2_MODULE_BUILD_DIR}/Logic
)

# -----------------------------------------------------------------------------
set(${KIT}_SRCS
  vtkSlicerPointSubscriberLogic.cxx
  vtkSlicerPointSubscriberLogic.h
)

# -----------------------------------------------------------------------------
# Libraries required by this logic module
set(${KIT}_TARGET_LIBRARIES
  ${ROS2_MODULE_BUILD_DIR}/lib/Slicer-5.6/qt-loadable-modules/libvtkSlicerROS2ModuleMRML.so
  ${ROS2_MODULE_BUILD_DIR}/lib/Slicer-5.6/qt-loadable-modules/libvtkSlicerROS2ModuleLogic.so

  # Slicer components
  vtkSlicerMarkupsModuleMRML
  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}

  # ROS 2 modern linkage (no more manual include dirs)
  rclcpp::rclcpp
  std_msgs::std_msgs
  geometry_msgs::geometry_msgs
  service_msgs::service_msgs
  rcl_interfaces::rcl_interfaces
  rcpputils::rcpputils
)

# -----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
)
