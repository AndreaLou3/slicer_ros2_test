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
#include <vtkSlicerPointSubscriberLogic.h>

// PointSubscriber includes
#include "qSlicerPointSubscriberModule.h"
#include "qSlicerPointSubscriberModuleWidget.h"

//-----------------------------------------------------------------------------
class qSlicerPointSubscriberModulePrivate
{
public:
  qSlicerPointSubscriberModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPointSubscriberModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModulePrivate::qSlicerPointSubscriberModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPointSubscriberModule methods

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModule::qSlicerPointSubscriberModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerPointSubscriberModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModule::~qSlicerPointSubscriberModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerPointSubscriberModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerPointSubscriberModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPointSubscriberModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerPointSubscriberModule::icon() const
{
  return QIcon(":/Icons/PointSubscriber.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerPointSubscriberModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPointSubscriberModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberModule::setup()
{
  this->Superclass::setup();
  
  std::cout << "=== PointSubscriber Module Setup Called ===" << std::endl;
  
  // Initialize the subscriber when module loads
  vtkSlicerPointSubscriberLogic* logic = 
    vtkSlicerPointSubscriberLogic::SafeDownCast(this->logic());
  
  if (!logic)
  {
    std::cerr << "ERROR: Failed to get logic!" << std::endl;
    return;
  }
  
  std::cout << "Logic obtained, calling InitializeSubscriber..." << std::endl;
  logic->InitializeSubscriber();
  std::cout << "InitializeSubscriber called" << std::endl;
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerPointSubscriberModule
::createWidgetRepresentation()
{
  return new qSlicerPointSubscriberModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerPointSubscriberModule::createLogic()
{
  return vtkSlicerPointSubscriberLogic::New();
}
