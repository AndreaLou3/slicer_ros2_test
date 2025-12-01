cmake_minimum_required(VERSION 3.13.4)

# Tell CMake where your Slicer build is
set(Slicer_DIR "/home/andrealou3/Slicer-source/build/Slicer-build")

# Find Slicer
find_package(Slicer REQUIRED)

# Print the variables
message(STATUS "Slicer_USE_FILE = ${Slicer_USE_FILE}")
message(STATUS "Slicer_EXTENSION_CMAKE = ${Slicer_EXTENSION_CMAKE}")

# Try including the extension macros (should succeed)
if(EXISTS ${Slicer_EXTENSION_CMAKE})
    message(STATUS "Including Slicer extension macros...")
    include(${Slicer_EXTENSION_CMAKE})
else()
    message(FATAL_ERROR "Slicer extension macros not found at ${Slicer_EXTENSION_CMAKE}")
endif()
