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

#ifndef __vtkSlicerPointSubscriberLogic_h
#define __vtkSlicerPointSubscriberLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes
#include <vtkMRMLROS2SubscriberNode.h>
#include <vtkMRMLROS2PublisherNode.h>
#include <vtkMRMLMarkupsFiducialNode.h>

// VTK includes
#include <vtkCallbackCommand.h>
#include <vtkSmartPointer.h>
#include <vtkTimerLog.h>

// STD includes
#include <cstdlib>

#include "vtkSlicerPointSubscriberModuleLogicExport.h"

class VTK_SLICER_POINTSUBSCRIBER_MODULE_LOGIC_EXPORT vtkSlicerPointSubscriberLogic :
  public vtkSlicerModuleLogic
{
public:
  static vtkSlicerPointSubscriberLogic *New();
  vtkTypeMacro(vtkSlicerPointSubscriberLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  // Initialize the ROS2 subscriber
  void InitializeSubscriber();

  // Initialize the ROS2 publisher for target point
  void InitializePublisher();

  // Start/Stop publishing target point periodically
  void StartPublishing(double intervalMs = 100.0);  // Default 100ms = 10Hz
  void StopPublishing();

  // Manually publish the current target point (can be called anytime)
  void PublishTargetPoint();

protected:
  vtkSlicerPointSubscriberLogic();
  ~vtkSlicerPointSubscriberLogic() override;

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  void RegisterNodes() override;
  void UpdateFromMRMLScene() override;
  void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;

  vtkMRMLROS2SubscriberNode* PointSubscriberNode = nullptr;
  vtkMRMLROS2PublisherNode* TargetPointPublisher = nullptr;  // Changed type!

private:
  vtkSlicerPointSubscriberLogic(const vtkSlicerPointSubscriberLogic&) = delete;
  void operator=(const vtkSlicerPointSubscriberLogic&) = delete;

  // Callback for subscriber (incoming points)
  void ProcessMRMLCallbacks(vtkObject* caller, unsigned long eid, void* callData);

  // Update the fiducial marker with received point
  void UpdateFiducial(double xyz[3]);

  // Get the coordinates of the "ROS2_Target" fiducial
  // Returns true if found, false otherwise
  // If not found, point array is not modified
  bool GetTargetPointCoordinates(double point[3]);

  // Timer callback for periodic publishing
  static void PublishTimerCallback(vtkObject* caller, unsigned long eid, 
                                   void* clientData, void* callData);

  // // ROS2 subscriber node
  // vtkMRMLROS2SubscriberNode* PointSubscriberNode = nullptr;

  // // ROS2 publisher node for target point
  // vtkMRMLROS2PublisherNode* TargetPointPublisher = nullptr;

  // Fiducial node for visualization
  vtkMRMLMarkupsFiducialNode* FiducialNode = nullptr;

  // Timer for periodic publishing
  vtkSmartPointer<vtkCallbackCommand> PublishTimer;
  unsigned long PublishTimerId = 0;
  double PublishInterval = 100.0;  // milliseconds
  double LastPublishTime = 0.0;
  
  // Track if we've already initialized
  bool Initialized = false;
};

#endif