/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerROSPointSubscriberModuleFooBarWidget.h"
#include "ui_qSlicerROSPointSubscriberModuleFooBarWidget.h"

//-----------------------------------------------------------------------------
class qSlicerROSPointSubscriberModuleFooBarWidgetPrivate
  : public Ui_qSlicerROSPointSubscriberModuleFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerROSPointSubscriberModuleFooBarWidget);
protected:
  qSlicerROSPointSubscriberModuleFooBarWidget* const q_ptr;

public:
  qSlicerROSPointSubscriberModuleFooBarWidgetPrivate(
    qSlicerROSPointSubscriberModuleFooBarWidget& object);
  virtual void setupUi(qSlicerROSPointSubscriberModuleFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerROSPointSubscriberModuleFooBarWidgetPrivate
::qSlicerROSPointSubscriberModuleFooBarWidgetPrivate(
  qSlicerROSPointSubscriberModuleFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerROSPointSubscriberModuleFooBarWidgetPrivate
::setupUi(qSlicerROSPointSubscriberModuleFooBarWidget* widget)
{
  this->Ui_qSlicerROSPointSubscriberModuleFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerROSPointSubscriberModuleFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerROSPointSubscriberModuleFooBarWidget
::qSlicerROSPointSubscriberModuleFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerROSPointSubscriberModuleFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerROSPointSubscriberModuleFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerROSPointSubscriberModuleFooBarWidget
::~qSlicerROSPointSubscriberModuleFooBarWidget()
{
}
