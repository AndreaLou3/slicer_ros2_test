set(SRCS
  vtkMRMLROS2PointStampedSubscriberNode.cxx
  vtkMRMLROS2PointStampedSubscriberNodeInternals.cxx
)

set(HDRS
  vtkMRMLROS2PointStampedSubscriberNode.h
  vtkMRMLROS2PointStampedSubscriberNodeInternals.h
)

add_library(${PROJECT_NAME}Logic ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME}Logic
  PUBLIC
    MRMLCore
    slicer_ros2_module
)

target_include_directories(${PROJECT_NAME}Logic
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Export logic library to parent
set(${PROJECT_NAME}_LOGIC_TARGET ${PROJECT_NAME}Logic PARENT_SCOPE)
