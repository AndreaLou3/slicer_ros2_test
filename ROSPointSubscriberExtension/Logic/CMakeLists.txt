set(Logic_SRCS
    vtkMRMLROS2PointStampedSubscriberNode.cxx
    vtkMRMLROS2PointStampedSubscriberNodeInternals.cxx
)

set(Logic_HDRS
    vtkMRMLROS2PointStampedSubscriberNode.h
    vtkMRMLROS2PointStampedSubscriberNodeInternals.h
)

add_library(Logic ${Logic_SRCS} ${Logic_HDRS})

target_link_libraries(Logic
    PUBLIC
        slicer_ros2_module
        MRMLCore
)

target_include_directories(Logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

