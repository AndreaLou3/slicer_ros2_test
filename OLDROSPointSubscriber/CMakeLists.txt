cmake_minimum_required(VERSION 3.13.4)

project(ROSPointSubscriberExtension)

# 1. Load Slicer
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

# 2. Extension metadata
set(EXTENSION_NAME "ROSPointSubscriberExtension")
set(EXTENSION_HOMEPAGE "none")
set(EXTENSION_CATEGORY "ROS")
set(EXTENSION_DESCRIPTION "ROS2 PointStamped Subscriber Module")
set(EXTENSION_CONTRIBUTORS "Andrea Lou")
set(EXTENSION_DEPENDS "slicer_ros2_module")  # must match actual dependency

# 3. Include Slicer extension macros
include(${Slicer_EXTENSION_CMAKE})

# 4. Add logic and module
add_subdirectory(Logic)
add_subdirectory(ROSPointSubscriberExtension)

# 5. Install extension
slicer_extension_install_all()
