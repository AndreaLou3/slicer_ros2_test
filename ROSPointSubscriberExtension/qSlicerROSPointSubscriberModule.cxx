#include "qSlicerROSPointSubscriberModule.h"

#include "vtkMRMLROS2PointStampedSubscriberNode.h"
#include "vtkMRMLScene.h"
#include <vtkNew.h>

qSlicerROSPointSubscriberModule
::qSlicerROSPointSubscriberModule(QObject* parent)
  : qSlicerLoadableModule(parent)
{
}

void qSlicerROSPointSubscriberModule::setup()
{
  this->Superclass::setup();

  // Register the MRML node so Slicer knows it exists
  vtkNew<vtkMRMLROS2PointStampedSubscriberNode> node;
  this->mrmlScene()->RegisterNodeClass(node);

  // --- Create instance of the subscriber ---
  vtkNew<vtkMRMLROS2PointStampedSubscriberNode> sub;
  sub->SetName("AutoPointSubscriber");
  sub->SetTopicName("/point_coords");
  scene->AddNode(sub);
}

