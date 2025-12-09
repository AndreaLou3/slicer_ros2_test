project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE
    "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

# -----------------------------------------------------------------------------
# Paths to the generated ROS2 MRML/Logic node code
set(ROS2_MODULE_SOURCE_DIR "FILL IN")
set(ROS2_MODULE_BUILD_DIR  "FILL IN")

# -----------------------------------------------------------------------------
# ROS 2 dependencies
find_package(rclcpp REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(service_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rcpputils REQUIRED)
find_package(rosbag2_interfaces REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(tf2_ros REQUIRED)


# -----------------------------------------------------------------------------
# Include dirs for Slicer
include_directories(${tf2_ros_INCLUDE_DIRS})
set(${KIT}_INCLUDE_DIRECTORIES
  ${ROS2_MODULE_SOURCE_DIR}/MRML
  ${ROS2_MODULE_SOURCE_DIR}/Logic
  ${ROS2_MODULE_BUILD_DIR}/MRML
  ${ROS2_MODULE_BUILD_DIR}/Logic
  ${tf2_ros_INCLUDE_DIRS}
)

# Also include the ROS2 install tree
string(REPLACE ":" ";" AMENT_PATHS "$ENV{AMENT_PREFIX_PATH}")
list(GET AMENT_PATHS 0 AMENT_FIRST_PATH)
list(APPEND ${KIT}_INCLUDE_DIRECTORIES "${AMENT_FIRST_PATH}/include")

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

  # ROS 2 C++ message libraries
  rclcpp::rclcpp
  rcpputils::rcpputils
  std_msgs::std_msgs__rosidl_generator_cpp
  geometry_msgs::geometry_msgs__rosidl_generator_cpp
  service_msgs::service_msgs__rosidl_generator_cpp
  sensor_msgs::sensor_msgs__rosidl_generator_cpp
  rcl_interfaces::rcl_interfaces__rosidl_generator_cpp
  std_srvs::std_srvs__rosidl_generator_cpp
  rosbag2_interfaces::rosbag2_interfaces__rosidl_generator_cpp
  moveit_msgs::moveit_msgs__rosidl_generator_cpp
  turtlesim::turtlesim__rosidl_generator_cpp
)

# -----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
)
