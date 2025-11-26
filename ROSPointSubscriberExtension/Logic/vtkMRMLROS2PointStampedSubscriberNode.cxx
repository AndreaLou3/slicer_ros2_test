#include "vtkMRMLROS2PointStampedSubscriberNode.h"
#include "vtkMRMLROS2PointStampedSubscriberNodeInternals.h"

#include "vtkMRMLMarkupsFiducialNode.h"
#include "vtkMRMLScene.h"
#include <vtkNew.h>

vtkMRMLNodeNewMacro(vtkMRMLROS2PointStampedSubscriberNode);

vtkMRMLROS2PointStampedSubscriberNode
::vtkMRMLROS2PointStampedSubscriberNode()
{
  this->Internals =
    new vtkMRMLROS2PointStampedSubscriberNodeInternals(this);
}

vtkMRMLROS2PointStampedSubscriberNode
::~vtkMRMLROS2PointStampedSubscriberNode()
{
  delete this->Internals;
}

void vtkMRMLROS2PointStampedSubscriberNode
::ProcessMessage(const geometry_msgs::msg::PointStamped& msg)
{
  vtkMRMLScene* scene = this->GetScene();
  if (!scene)
    return;

  // Create a single fiducial (or update it if already exists)
  vtkMRMLMarkupsFiducialNode* fid =
      vtkMRMLMarkupsFiducialNode::SafeDownCast(
          scene->GetFirstNodeByName("ROSPoint"));

  if (!fid)
  {
    vtkNew<vtkMRMLMarkupsFiducialNode> newFid;
    newFid->SetName("ROSPoint");
    fid = newFid;
    scene->AddNode(fid);
  }

  const auto& p = msg.point;
  if (fid->GetNumberOfControlPoints() == 0) {
    fid->AddControlPointWorld(vtkVector3d(p.x, p.y, p.z));
  } else {
    fid->SetNthControlPointPositionWorld(0, p.x, p.y, p.z);
  }

  auto red   = vtkMRMLSliceNode::SafeDownCast(scene->GetNodeByID("vtkMRMLSliceNodeRed"));
  auto yellow = vtkMRMLSliceNode::SafeDownCast(scene->GetNodeByID("vtkMRMLSliceNodeYellow"));
  auto green  = vtkMRMLSliceNode::SafeDownCast(scene->GetNodeByID("vtkMRMLSliceNodeGreen"));

  if (red)    red->SetSliceOffset(p.x);
  if (yellow) yellow->SetSliceOffset(p.y);
  if (green)  green->SetSliceOffset(p.z);

}

