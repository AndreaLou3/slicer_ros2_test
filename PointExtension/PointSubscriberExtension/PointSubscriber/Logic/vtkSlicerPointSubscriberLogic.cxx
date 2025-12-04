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

#include <vtkMRMLScene.h>
#include <vtkMRMLROS2NodeNode.h>
#include <vtkMRMLROS2SubscriberNode.h>
#include <vtkMRMLMarkupsFiducialNode.h>


// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerPointSubscriberLogic);

//----------------------------------------------------------------------------
vtkSlicerPointSubscriberLogic::vtkSlicerPointSubscriberLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerPointSubscriberLogic::~vtkSlicerPointSubscriberLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerPointSubscriberLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
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
void vtkSlicerPointSubscriberLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

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
      displayNode->SetGlyphScale(1.0);
      displayNode->SetSelectedColor(1.0, 0.0, 0.0);
    }
    
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

// //------------------------------------------------------------------------------
// void vtkSlicerPointSubscriberLogic::UpdateFiducial(double xyz[3])
// {
//   vtkInfoMacro("UpdateFiducial called with: [" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << "]");
  
//   if (!this->FiducialNode)
//   {
//     vtkInfoMacro("Creating new fiducial node...");
//     this->FiducialNode = vtkMRMLMarkupsFiducialNode::New();
//     this->FiducialNode->SetName("ROS2_Point");
//     this->GetMRMLScene()->AddNode(this->FiducialNode);
//     this->FiducialNode->AddFiducial(0, 0, 0);
//     vtkInfoMacro("Fiducial node created and added to scene");
//   }

//   this->FiducialNode->SetNthControlPointPosition(0, xyz[0], xyz[1], xyz[2]);
//   vtkInfoMacro("Fiducial position updated");
// }
