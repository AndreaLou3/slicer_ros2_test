/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// PointSubscriber Logic includes
#include "vtkSlicerPointSubscriberLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>

#include <vtkMRMLScene.h>
#include <vtkMRMLROS2NodeNode.h>
#include <vtkMRMLROS2SubscriberNode.h>
#include <vtkMRMLROS2PublisherNode.h>
#include <vtkMRMLROS2GeneratedNodes.h>
#include <vtkMRMLMarkupsFiducialNode.h>
#include "vtkMRMLROS2PublisherInternals.h"
#include <vtkMRMLROS2SubscriberDefaultNodes.h>
#include <vtkMRMLROS2PublisherDefaultNodes.h>

#include <vtkMRMLROS2GeneratedNodes.h>

// STD includes
#include <cassert>
#include <cmath>

#include <vtkSlicerROS2Logic.h>
#include <qSlicerApplication.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerPointSubscriberLogic);

//----------------------------------------------------------------------------
vtkSlicerPointSubscriberLogic::vtkSlicerPointSubscriberLogic()
{
  this->PublishTimer = vtkSmartPointer<vtkCallbackCommand>::New();
  this->PublishTimer->SetCallback(PublishTimerCallback);
  this->PublishTimer->SetClientData(this);
}

//----------------------------------------------------------------------------
vtkSlicerPointSubscriberLogic::~vtkSlicerPointSubscriberLogic()
{
  this->StopPublishing();
}

//----------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
// void vtkSlicerPointSubscriberLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
// {
//   vtkNew<vtkIntArray> events;
//   events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
//   events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
//   events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
//   this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
  
//   // Initialize ROS2 subscriber and publisher when scene is set
//   if (newScene && !this->Initialized)
//   {
//     vtkInfoMacro("Scene is now available, initializing ROS2 nodes...");
//     this->InitializeSubscriber();
//     this->InitializePublisher();
//     this->StartPublishing(100.0);
//     this->Initialized = true;
//     vtkInfoMacro("ROS2 initialization complete");
//   }
// }
//----------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());

  // Reset initialization flag
  this->Initialized = false;
}

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::OnMRMLSceneNodeAdded(vtkMRMLNode* node)
{
    if (this->Initialized)
        return;

    auto rosNode = vtkMRMLROS2NodeNode::SafeDownCast(node);
    if (!rosNode)
        return;

    // Get ROS2 module logic
    vtkSlicerROS2Logic* rosLogic =
        vtkSlicerROS2Logic::SafeDownCast(
            qSlicerCoreApplication::application()->moduleLogic("ROS2"));

    if (!rosLogic)
    {
        vtkWarningMacro("ROS2 module logic not available yet, try again later");
        return;
    }

    // **Do NOT call RegisterNodes() manually** — ROS2 logic already registers default nodes

    // Now it’s safe to create your subscriber/publisher
    this->InitializeSubscriber();
    this->InitializePublisher();
    this->StartPublishing(100.0);
    this->Initialized = true;

    vtkInfoMacro("ROS2 initialization complete");
}

//-----------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
// void vtkSlicerPointSubscriberLogic
// ::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
// {
// }

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::InitializeSubscriber()
{
  vtkMRMLScene* scene = this->GetMRMLScene();
  if (!scene)
  {
    vtkErrorMacro("Cannot initialize subscriber: no scene found.");
    return;
  }

  // Find the active ROS2 node in the scene
  auto rosNode = vtkMRMLROS2NodeNode::SafeDownCast(
      scene->GetFirstNodeByClass("vtkMRMLROS2NodeNode"));
  if (!rosNode)
  {
    vtkErrorMacro("No ROS2 node exists! Start the Slicer ROS2 module first.");
    return;
  }

  // Create a DoubleArray subscriber for xyz
  auto sub = rosNode->CreateAndAddSubscriberNode("DoubleArray", "/my_point_topic");
  if (!sub)
  {
    vtkErrorMacro("Failed to create subscriber!");
    return;
  }

  // Observe changes
  sub->AddObserver(vtkCommand::ModifiedEvent, this,
    &vtkSlicerPointSubscriberLogic::ProcessMRMLCallbacks);

  this->PointSubscriberNode = sub;

  vtkInfoMacro("Point subscriber initialized.");
}

//---------------------------------------------------------------------------
// Minimal helper wrapper to expose Publish
class PointPublisherWrapper : public vtkMRMLROS2PublisherNode
{
public:
  size_t PublishDoubleArray(vtkDoubleArray* arr)
  {
    auto internalsVTK = dynamic_cast<
        vtkMRMLROS2PublisherVTKInternals<vtkDoubleArray, std_msgs::msg::Float64MultiArray>*>(
            this->mInternals);  // protected access allowed in subclass
    if (!internalsVTK)
    {
      vtkErrorMacro("Failed to cast publisher internals to VTK internals!");
      return 0;
    }
    return internalsVTK->Publish(arr);
  }
};


//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::InitializePublisher()
{
  vtkMRMLScene* scene = this->GetMRMLScene();
  if (!scene)
  {
    vtkErrorMacro("Cannot initialize publisher: no scene found.");
    return;
  }

  // Find the active ROS2 node in the scene
  auto rosNode = vtkMRMLROS2NodeNode::SafeDownCast(
      scene->GetFirstNodeByClass("vtkMRMLROS2NodeNode"));
  if (!rosNode)
  {
    vtkErrorMacro("No ROS2 node exists! Start the Slicer ROS2 module first.");
    return;
  }

  // Create a DoubleArray publisher for target point coordinates
  this->TargetPointPublisher = rosNode->CreateAndAddPublisherNode("DoubleArray", "/get_target_point");

  if (!this->TargetPointPublisher)
  {
    vtkErrorMacro("Failed to create publisher!");
    return;
  }
  
  this->TargetPointPublisherWrapper = std::make_unique<PointPublisherWrapper>(this->TargetPointPublisher);
  vtkInfoMacro("Target point publisher initialized on topic /get_target_point");
}

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::StartPublishing(double intervalMs)
{
  if (this->PublishTimerId != 0)
  {
    vtkWarningMacro("Publisher timer already running!");
    return;
  }

  if (!this->TargetPointPublisher)
  {
    vtkErrorMacro("Publisher not initialized! Call InitializePublisher() first.");
    return;
  }

  // Create a timer to periodically publish the target point
  // this->PublishTimerId = this->GetMRMLScene()->AddObserver(
  //   vtkMRMLScene::EndBatchProcessEvent, this->PublishTimer);
  
  this->PublishInterval = intervalMs;
  this->LastPublishTime = vtkTimerLog::GetUniversalTime();

  vtkInfoMacro("Started publishing target point every " << intervalMs << " ms");
}

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::StopPublishing()
{
  if (this->PublishTimerId != 0 && this->GetMRMLScene())
  {
    this->GetMRMLScene()->RemoveObserver(this->PublishTimerId);
    this->PublishTimerId = 0;
    vtkInfoMacro("Stopped publishing target point");
  }
}

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::PublishTimerCallback(
    vtkObject* caller, unsigned long, void* clientData, void*)
{
  vtkSlicerPointSubscriberLogic* self = 
    reinterpret_cast<vtkSlicerPointSubscriberLogic*>(clientData);
  
  if (!self)
  {
    return;
  }

  // Check if enough time has elapsed
  double currentTime = vtkTimerLog::GetUniversalTime();
  double elapsedMs = (currentTime - self->LastPublishTime) * 1000.0;
  
  if (elapsedMs >= self->PublishInterval)
  {
    self->PublishTargetPoint();
    self->LastPublishTime = currentTime;
  }
}


//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::PublishTargetPoint()
{
  if (!this->TargetPointPublisherWrapper)
  {
    vtkErrorMacro("Publisher wrapper not initialized!");
    return;
  }

  double point[3];
  bool found = this->GetTargetPointCoordinates(point);

  if (!found)
  {
    point[0] = point[1] = point[2] = std::numeric_limits<double>::quiet_NaN();
    vtkDebugMacro("Target point not found, publishing NaN");
  }
  else
  {
    vtkDebugMacro("Publishing target point: [" << point[0] << ", " 
                  << point[1] << ", " << point[2] << "]");
  }

  vtkNew<vtkDoubleArray> arr;
  arr->SetNumberOfComponents(3);
  arr->SetNumberOfTuples(1);
  arr->SetTuple(0, point);

  // Use the wrapper to publish
  this->TargetPointPublisherWrapper->PublishDoubleArray(arr.GetPointer());
}

//---------------------------------------------------------------------------
bool vtkSlicerPointSubscriberLogic::GetTargetPointCoordinates(double point[3])
{
  vtkMRMLScene* scene = this->GetMRMLScene();
  if (!scene)
  {
    vtkErrorMacro("No scene available!");
    return false;
  }

  // Search for a markup node named "ROS2_Target"
  vtkMRMLNode* node = scene->GetFirstNodeByName("ROS2_Target");
  if (!node)
  {
    vtkDebugMacro("No node named 'ROS2_Target' found in scene");
    return false;
  }

  // Try to cast to a fiducial node
  vtkMRMLMarkupsFiducialNode* fiducialNode = 
    vtkMRMLMarkupsFiducialNode::SafeDownCast(node);
  
  if (!fiducialNode)
  {
    vtkWarningMacro("Node 'ROS2_Target' exists but is not a fiducial node");
    return false;
  }

  // Check if it has any control points
  if (fiducialNode->GetNumberOfControlPoints() == 0)
  {
    vtkDebugMacro("Fiducial 'ROS2_Target' has no control points");
    return false;
  }

  // Get the first control point position
  fiducialNode->GetNthControlPointPosition(0, point);
  
  return true;
}

//------------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::ProcessMRMLCallbacks(
    vtkObject* caller, unsigned long, void*)
{
  vtkInfoMacro("ProcessMRMLCallbacks called!");
  
  auto* sub = vtkMRMLROS2SubscriberNode::SafeDownCast(caller);
  if (!sub)
  {
    vtkErrorMacro("Caller is not a subscriber node!");
    return;
  }

  vtkInfoMacro("Getting last message variant...");
  vtkVariant variant = sub->GetLastMessageVariant();
  
  vtkDoubleArray* arr = vtkDoubleArray::SafeDownCast(variant.ToVTKObject());
  if (!arr)
  {
    vtkErrorMacro("Failed to cast to vtkDoubleArray!");
    return;
  }
  
  vtkInfoMacro("Array has " << arr->GetNumberOfTuples() << " tuples and " 
               << arr->GetNumberOfComponents() << " components");
  
  if (arr->GetNumberOfValues() < 3)
  {
    vtkErrorMacro("Array doesn't have enough data!");
    return;
  }

  double point[3];
  arr->GetTuple(0, point);
  
  vtkInfoMacro("Received point: [" << point[0] << ", " << point[1] << ", " << point[2] << "]");
  
  this->UpdateFiducial(point);
}

//------------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::UpdateFiducial(double xyz[3])
{
  vtkInfoMacro("UpdateFiducial called with: [" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << "]");
  
  if (!this->FiducialNode)
  {
    vtkInfoMacro("Creating new fiducial node...");
    this->FiducialNode = vtkMRMLMarkupsFiducialNode::New();
    this->FiducialNode->SetName("ROS2_Point");
    this->GetMRMLScene()->AddNode(this->FiducialNode);

    // Make it visible and styled
    this->FiducialNode->SetDisplayVisibility(true);
    
    vtkMRMLMarkupsDisplayNode* displayNode = 
      vtkMRMLMarkupsDisplayNode::SafeDownCast(this->FiducialNode->GetDisplayNode());
    if (displayNode)
    {
      displayNode->SetVisibility(true);
      displayNode->SetVisibility2D(true);
      displayNode->SetGlyphScale(1.0);
      displayNode->SetSelectedColor(1.0, 0.0, 0.0);
    }

    // Fix: make point visible in 2D slice views with volumes loaded
    displayNode->SetSliceProjection(true);
    displayNode->SetSliceProjectionUseFiducialColor(true);
    // Show outline when behind
    displayNode->SetSliceProjectionOutlinedBehindSlicePlane(true);
    displayNode->SetSliceProjectionColor(1.0, 0.0, 0.0);
    displayNode->SetSliceProjectionOpacity(1.0);
    
    vtkInfoMacro("Fiducial node created");
  }
  
  // Update or add the control point with valid data
  if (this->FiducialNode->GetNumberOfControlPoints() == 0)
  {
    // First time - add the control point
    this->FiducialNode->AddControlPoint(xyz[0], xyz[1], xyz[2]);
    vtkInfoMacro("Control point added at: [" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << "]");
  }
  else
  {
    // Update existing control point
    this->FiducialNode->SetNthControlPointPosition(0, xyz[0], xyz[1], xyz[2]);
    vtkInfoMacro("Control point updated to: [" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << "]");
  }
}