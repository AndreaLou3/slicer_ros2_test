project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

#-----------------------------------------------------------------------------
# Set paths to ROS2 module source and build directories
set(ROS2_MODULE_SOURCE_DIR "/home/andrealou3/ros2_ws/src/slicer_ros2_module")
set(ROS2_MODULE_BUILD_DIR "/home/andrealou3/ros2_ws/src/slicer_ros2_module/build")

# Get ROS2 include directories from environment
# These should be set when ROS2 is sourced
set(ROS2_INCLUDE_DIRS
  "$ENV{AMENT_PREFIX_PATH}/include/rclcpp"
  "$ENV{AMENT_PREFIX_PATH}/include/rcl"
  "$ENV{AMENT_PREFIX_PATH}/include/std_msgs"
  "$ENV{AMENT_PREFIX_PATH}/include/geometry_msgs"
  )

# If AMENT_PREFIX_PATH has multiple paths (separated by :), take the first one
string(REPLACE ":" ";" AMENT_PATHS "$ENV{AMENT_PREFIX_PATH}")
list(GET AMENT_PATHS 0 AMENT_FIRST_PATH)

set(${KIT}_INCLUDE_DIRECTORIES
  # Source directories (where the .h files are)
  ${ROS2_MODULE_SOURCE_DIR}/MRML
  ${ROS2_MODULE_SOURCE_DIR}/Logic
  # Build directories (for generated export headers)
  ${ROS2_MODULE_BUILD_DIR}/MRML
  ${ROS2_MODULE_BUILD_DIR}/Logic
  # ROS2 include directories
  ${AMENT_FIRST_PATH}/include/rclcpp
  ${AMENT_FIRST_PATH}/include/rcl
  ${AMENT_FIRST_PATH}/include/rcl_interfaces
  ${AMENT_FIRST_PATH}/include/rcl_yaml_param_parser
  ${AMENT_FIRST_PATH}/include/rcutils
  ${AMENT_FIRST_PATH}/include/rmw
  ${AMENT_FIRST_PATH}/include/rosidl_runtime_c
  ${AMENT_FIRST_PATH}/include/rosidl_runtime_cpp
  ${AMENT_FIRST_PATH}/include/rosidl_typesupport_interface
  ${AMENT_FIRST_PATH}/include/rosidl_typesupport_cpp
  ${AMENT_FIRST_PATH}/include/std_msgs
  ${AMENT_FIRST_PATH}/include/geometry_msgs
  ${AMENT_FIRST_PATH}/include/builtin_interfaces
  ${AMENT_FIRST_PATH}/include/service_msgs
  )

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  vtkSlicerPointSubscriberLogic.cxx
  vtkSlicerPointSubscriberLogic.h
  )

#-----------------------------------------------------------------------------
set(${KIT}_TARGET_LIBRARIES
  ${ROS2_MODULE_BUILD_DIR}/lib/Slicer-5.6/qt-loadable-modules/libvtkSlicerROS2ModuleMRML.so
  ${ROS2_MODULE_BUILD_DIR}/lib/Slicer-5.6/qt-loadable-modules/libvtkSlicerROS2ModuleLogic.so
  vtkSlicerMarkupsModuleMRML 
  ${VTK_LIBRARIES}
  ${ITK_LIBRARIES}
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )