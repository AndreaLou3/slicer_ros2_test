cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)

project(PointSubscriberExtension)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "https://www.slicer.org/wiki/Documentation/Nightly/Extensions/PointSubscriberExtension")
set(EXTENSION_CATEGORY "Examples")
set(EXTENSION_CONTRIBUTORS "John Doe (AnyWare Corp.)")
set(EXTENSION_DESCRIPTION "This is an example of a simple extension")
set(EXTENSION_ICONURL "https://www.example.com/Slicer/Extensions/PointSubscriberExtension.png")
set(EXTENSION_SCREENSHOTURLS "https://www.example.com/Slicer/Extensions/PointSubscriberExtension/Screenshots/1.png")
set(EXTENSION_DEPENDS "slicer_ros2_module") # Specified as a list or "NA" if no dependencies

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

# # Find the ROS2 module
# find_package(slicer_ros2_module REQUIRED)

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(PointSubscriber)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_GENERATE_CONFIG})
include(${Slicer_EXTENSION_CPACK})
