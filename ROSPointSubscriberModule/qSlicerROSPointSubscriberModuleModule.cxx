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

// ROSPointSubscriberModule Logic includes
#include <vtkSlicerROSPointSubscriberModuleLogic.h>

// ROSPointSubscriberModule includes
#include "qSlicerROSPointSubscriberModuleModule.h"
#include "qSlicerROSPointSubscriberModuleModuleWidget.h"

//-----------------------------------------------------------------------------
class qSlicerROSPointSubscriberModuleModulePrivate
{
public:
  qSlicerROSPointSubscriberModuleModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerROSPointSubscriberModuleModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerROSPointSubscriberModuleModulePrivate::qSlicerROSPointSubscriberModuleModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerROSPointSubscriberModuleModule methods

//-----------------------------------------------------------------------------
qSlicerROSPointSubscriberModuleModule::qSlicerROSPointSubscriberModuleModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerROSPointSubscriberModuleModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerROSPointSubscriberModuleModule::~qSlicerROSPointSubscriberModuleModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerROSPointSubscriberModuleModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerROSPointSubscriberModuleModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerROSPointSubscriberModuleModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerROSPointSubscriberModuleModule::icon() const
{
  return QIcon(":/Icons/ROSPointSubscriberModule.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerROSPointSubscriberModuleModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerROSPointSubscriberModuleModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerROSPointSubscriberModuleModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerROSPointSubscriberModuleModule
::createWidgetRepresentation()
{
  return new qSlicerROSPointSubscriberModuleModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerROSPointSubscriberModuleModule::createLogic()
{
  return vtkSlicerROSPointSubscriberModuleLogic::New();
}
