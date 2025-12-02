project(vtkSlicer${MODULE_NAME}ModuleLogic)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_LOGIC_EXPORT")

#-----------------------------------------------------------------------------
set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkSlicerROS2ModuleMRML_INCLUDE_DIRS}
  ${vtkSlicerROS2ModuleLogic_INCLUDE_DIRS}
  )

#-----------------------------------------------------------------------------
set(${KIT}_SRCS
  vtkSlicer${MODULE_NAME}Logic.cxx
  vtkSlicer${MODULE_NAME}Logic.h
  )

#-----------------------------------------------------------------------------
set(${KIT}_TARGET_LIBRARIES
  vtkSlicerROS2ModuleMRML
  vtkSlicerROS2ModuleLogic
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
