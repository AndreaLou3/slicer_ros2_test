#pragma once

#include "vtkMRMLROS2SubscriberVTKInternals.h"
#include <geometry_msgs/msg/point_stamped.hpp>

class vtkMRMLROS2PointStampedSubscriberNode;

class vtkMRMLROS2PointStampedSubscriberNodeInternals
  : public vtkMRMLROS2SubscriberVTKInternals<
        geometry_msgs::msg::PointStamped,
        vtkMRMLROS2PointStampedSubscriberNode
    >
{
public:
  using Superclass = vtkMRMLROS2SubscriberVTKInternals<
        geometry_msgs::msg::PointStamped,
        vtkMRMLROS2PointStampedSubscriberNode>;

  vtkMRMLROS2PointStampedSubscriberNodeInternals(
      vtkMRMLROS2PointStampedSubscriberNode* external);
};

