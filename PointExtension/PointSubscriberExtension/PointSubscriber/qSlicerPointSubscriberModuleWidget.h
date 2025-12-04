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

#ifndef __qSlicerPointSubscriberModuleWidget_h
#define __qSlicerPointSubscriberModuleWidget_h

// Slicer includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerPointSubscriberModuleExport.h"

class qSlicerPointSubscriberModuleWidgetPrivate;
class vtkMRMLNode;

// ADDED: Forward declarations for click handling
class vtkObject;
class vtkCommand;

class Q_SLICER_QTMODULES_POINTSUBSCRIBER_EXPORT qSlicerPointSubscriberModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerPointSubscriberModuleWidget(QWidget *parent=0);
  virtual ~qSlicerPointSubscriberModuleWidget();

public slots:
  // ADDED: Slots for UI interactions
  void onStartSubscriberClicked();
  void onEnableClickPublishToggled(bool enabled);


protected:
  QScopedPointer<qSlicerPointSubscriberModuleWidgetPrivate> d_ptr;

  void setup() override;
  
  // ADDED: Methods for click handling
  void setupClickInteractor();
  void cleanupClickInteractor();

private:
  Q_DECLARE_PRIVATE(qSlicerPointSubscriberModuleWidget);
  Q_DISABLE_COPY(qSlicerPointSubscriberModuleWidget);

  // ADDED: Member variables for click interaction
  vtkCommand* ClickObserver;
  unsigned long ClickObserverTag;
};

#endif
