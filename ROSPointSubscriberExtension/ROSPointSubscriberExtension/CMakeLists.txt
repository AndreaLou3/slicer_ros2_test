set(MODULE_NAME "ROSPointSubscriberExtension")
set(MODULE_TITLE "ROS Point Subscriber")

set(MODULE_SRCS
  qSlicerROSPointSubscriberModule.cxx
)

set(MODULE_HDRS
  qSlicerROSPointSubscriberModule.h
)

# The module will link against the logic library we built earlier
set(MODULE_TARGET_LIBRARIES
  ${PROJECT_NAME}Logic
  MRMLCore
  MRMLLogic
  SlicerBaseLogic
)

slicerMacroBuildLoadableModule(
  NAME ${MODULE_NAME}
  TITLE ${MODULE_TITLE}
  SRCS ${MODULE_SRCS} ${MODULE_HDRS}
  TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
)
