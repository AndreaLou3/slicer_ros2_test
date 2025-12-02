project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

#-----------------------------------------------------------------------------
# Set paths to ROS2 module source and build directories
set(ROS2_MODULE_SOURCE_DIR "/home/andrealou3/ros2_ws/src/slicer_ros2_module")
set(ROS2_MODULE_BUILD_DIR "/home/andrealou3/ros2_ws/src/slicer_ros2_module/build")

set(${KIT}_INCLUDE_DIRECTORIES
  # Source directories (where the .h files are)
  ${ROS2_MODULE_SOURCE_DIR}/MRML
  ${ROS2_MODULE_SOURCE_DIR}/Logic
  # Build directories (for generated export headers)
  ${ROS2_MODULE_BUILD_DIR}/MRML
  ${ROS2_MODULE_BUILD_DIR}/Logic
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
