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

#include <QVBoxLayout>
#include <QPushButton> 
#include <QDebug>

//-----------------------------------------------------------------------------
class qSlicerPointSubscriberFooBarWidgetPrivate
  : public Ui_qSlicerPointSubscriberFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerPointSubscriberFooBarWidget);
protected:
  qSlicerPointSubscriberFooBarWidget* const q_ptr;

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
{
  this->PublishButton = 0;
}

// --------------------------------------------------------------------------
void qSlicerPointSubscriberFooBarWidgetPrivate
::setupUi(qSlicerPointSubscriberFooBarWidget* widget)
{
  // 1. Manually create the button
  this->PublishButton = new QPushButton(widget);
  this->PublishButton->setText(tr("Publish Target Point"));
  this->PublishButton->setToolTip(tr("Sends the coordinates of the 'ROS2_Target' markup point via ROS 2."));
  
  // 2. Set up the layout
  QVBoxLayout* layout = new QVBoxLayout(widget);
  layout->addWidget(this->PublishButton);
  layout->addStretch(1);

  // 3. Connect the signal to the slot
  QObject::connect(this->PublishButton, SIGNAL(clicked()),
                   widget, SLOT(onPublishButtonClicked()));

  // NOTE: If you are building manually, you should NOT call the UI setup function.
  // The line below should be removed or commented out:
  // this->Ui_qSlicerPointSubscriberFooBarWidget::setupUi(widget);
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
}

//-----------------------------------------------------------------------------
qSlicerPointSubscriberFooBarWidget
::~qSlicerPointSubscriberFooBarWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberFooBarWidget::setLogic(vtkSlicerPointSubscriberLogic* logic)
{
  this->Logic = logic;
}

// **CHANGE 3B: Implement onPublishButtonClicked slot**
void qSlicerPointSubscriberFooBarWidget::onPublishButtonClicked()
{
  if (!this->Logic)
  {
    qWarning() << Q_FUNC_INFO << ": Logic not set!";
    return;
  }
  
  // Call the public method in the Logic class
  this->Logic->PublishTargetPoint();
  qDebug() << Q_FUNC_INFO << ": Target point published.";
}