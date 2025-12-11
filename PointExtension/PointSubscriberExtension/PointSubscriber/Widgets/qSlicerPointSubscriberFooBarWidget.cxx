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
#include "qSlicerPointSubscriberFooBarWidget.h"
#include "ui_qSlicerPointSubscriberFooBarWidget.h"

// Need to include logic header and application header
#include "vtkSlicerPointSubscriberLogic.h" 
#include <qSlicerApplication.h>

//-----------------------------------------------------------------------------
class qSlicerPointSubscriberFooBarWidgetPrivate
  : public Ui_qSlicerPointSubscriberFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerPointSubscriberFooBarWidget);
protected:
  qSlicerPointSubscriberFooBarWidget* const q_ptr;
  vtkSlicerPointSubscriberLogic* Logic;

public:
  qSlicerPointSubscriberFooBarWidgetPrivate(
    qSlicerPointSubscriberFooBarWidget& object);
  virtual void setupUi(qSlicerPointSubscriberFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerPointSubscriberFooBarWidgetPrivate
::qSlicerPointSubscriberFooBarWidgetPrivate(
  qSlicerPointSubscriberFooBarWidget& object)
  : q_ptr(&object)
  , Logic(nullptr)
{
  // Retrieve the logic pointer here
  this->Logic = vtkSlicerPointSubscriberLogic::SafeDownCast(
      qSlicerCoreApplication::application()->moduleLogic("PointSubscriber"));
  
  if (!this->Logic)
  {
      qWarning() << "PointSubscriber module logic not found!";
  }
}

// --------------------------------------------------------------------------
void qSlicerPointSubscriberFooBarWidgetPrivate
::setupUi(qSlicerPointSubscriberFooBarWidget* widget)
{
  this->Ui_qSlicerPointSubscriberFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPointSubscriberFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerPointSubscriberFooBarWidget
::qSlicerPointSubscriberFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPointSubscriberFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerPointSubscriberFooBarWidget);
  d->setupUi(this);

  // Connecting button to logic
  // d->FooBarButton is available because its objectName is "FooBarButton" in the .ui file.
  // SIGNAL(clicked()) is the standard signal for a QPushButton.
  // SLOT(onFooBarButtonClicked()) is the function from the header.
  connect(d->FooBarButton, SIGNAL(clicked()),
          this, SLOT(onFooBarButtonClicked()));
}

// //-----------------------------------------------------------------------------
// qSlicerPointSubscriberFooBarWidget
// ::qSlicerPointSubscriberFooBarWidget(QWidget* parentWidget)
//   : Superclass( parentWidget )
//   , d_ptr( new qSlicerPointSubscriberFooBarWidgetPrivate(*this) )
// {
//   Q_D(qSlicerPointSubscriberFooBarWidget);
//   d->setupUi(this);
// }

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberFooBarWidget
::onFooBarButtonClicked()
{
  // This function is now executed every time the "Foo Bar" button is pressed.
  // **Place your ROS publishing logic here.**

  // For debugging, use qDebug (requires #include <QDebug> at the top):
  qDebug() << "--- Foo Bar Button Clicked! ---";
  
  Q_D(qSlicerPointSubscriberFooBarWidget);
  
  if (d->Logic)
  {
    // *** THE ONE-TIME PUBLISH CALL ***
    d->Logic->PublishTargetPoint();
    qInfo() << "ROS2 target coordinates published once.";
  }
  else
  {
    qWarning() << "Cannot publish: Module logic is null.";
  }
}

//-----------------------------------------------------------------------------
qSlicerPointSubscriberFooBarWidget
::~qSlicerPointSubscriberFooBarWidget()
{
}


