#pragma once

#include "vtkMRMLROS2SubscriberNode.h"

class vtkMRMLMarkupsFiducialNode;
class vtkMRMLROS2PointStampedSubscriberNodeInternals;

class vtkMRMLROS2PointStampedSubscriberNode
  : public vtkMRMLROS2SubscriberNode
{
public:
  static vtkMRMLROS2PointStampedSubscriberNode* New();
  vtkTypeMacro(vtkMRMLROS2PointStampedSubscriberNode, vtkMRMLROS2SubscriberNode);

  const char* GetNodeTagName() override { return "ROS2PointStampedSubscriber"; }

  void ProcessMessage(const geometry_msgs::msg::PointStamped& msg);

protected:
  vtkMRMLROS2PointStampedSubscriberNode();
  ~vtkMRMLROS2PointStampedSubscriberNode() override;

  vtkMRMLROS2PointStampedSubscriberNodeInternals* Internals;
};

