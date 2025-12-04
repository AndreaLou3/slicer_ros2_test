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

// Qt includes
#include <QDebug>

// Slicer includes
#include "qSlicerPointSubscriberModuleWidget.h"
#include "ui_qSlicerPointSubscriberModuleWidget.h"

#include "vtkSlicerPointSubscriberLogic.h"

#include <qSlicerApplication.h>
#include <qSlicerLayoutManager.h>
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>

// ADDED: Custom callback class for handling clicks
class ClickCallback : public vtkCommand
{
public:
  static ClickCallback* New() { return new ClickCallback; }
  
  void Execute(vtkObject* caller, unsigned long eventId, void* callData) override
  {
    if (eventId == vtkCommand::LeftButtonPressEvent && this->Widget)
    {
      this->Widget->handleClick();
    }
  }
  
  qSlicerPointSubscriberModuleWidget* Widget = nullptr;
};

//-----------------------------------------------------------------------------
class qSlicerPointSubscriberModuleWidgetPrivate: public Ui_qSlicerPointSubscriberModuleWidget
{
public:
  qSlicerPointSubscriberModuleWidgetPrivate();

  // ADDED: UI elements
  QPushButton* StartSubscriberButton;
  QCheckBox* EnableClickPublishCheckbox;
  QLabel* StatusLabel;
};

//-----------------------------------------------------------------------------
// qSlicerPointSubscriberModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModuleWidgetPrivate::qSlicerPointSubscriberModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPointSubscriberModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModuleWidget::qSlicerPointSubscriberModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPointSubscriberModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModuleWidget::~qSlicerPointSubscriberModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberModuleWidget::setup()
{
  Q_D(qSlicerPointSubscriberModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

    
  // ADDED: Create custom UI elements
  QVBoxLayout* layout = new QVBoxLayout();
  
  // Start Subscriber button
  d->StartSubscriberButton = new QPushButton("Start Point Subscriber");
  connect(d->StartSubscriberButton, SIGNAL(clicked()), this, SLOT(onStartSubscriberClicked()));
  layout->addWidget(d->StartSubscriberButton);
  
  // Enable click to publish checkbox
  d->EnableClickPublishCheckbox = new QCheckBox("Enable Click to Publish");
  d->EnableClickPublishCheckbox->setChecked(false);
  connect(d->EnableClickPublishCheckbox, SIGNAL(toggled(bool)), 
          this, SLOT(onEnableClickPublishToggled(bool)));
  layout->addWidget(d->EnableClickPublishCheckbox);
  
  // Status label
  d->StatusLabel = new QLabel("Status: Ready");
  layout->addWidget(d->StatusLabel);
  
  // Add layout to the widget
  this->layout()->addItem(layout);
  
  // Auto-start the subscriber
  vtkSlicerPointSubscriberLogic* logic = d->logic();
  if (logic)
  {
    logic->InitializeSubscriber();
    logic->InitializePublisher();
    d->StatusLabel->setText("Status: Subscriber and Publisher initialized");
  }

  // auto logic = vtkSlicerPointSubscriberLogic::SafeDownCast(this->logic());
  // if (logic)
  // {
  //   logic->InitializeSubscriber();
  // }
}

// new methods
//-----------------------------------------------------------------------------
void qSlicerPointSubscriberModuleWidget::onStartSubscriberClicked()
{
  Q_D(qSlicerPointSubscriberModuleWidget);
  
  vtkSlicerPointSubscriberLogic* logic = d->logic();
  if (logic)
  {
    logic->InitializeSubscriber();
    logic->InitializePublisher();
    d->StatusLabel->setText("Status: Subscriber and Publisher (re)initialized");
  }
}

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberModuleWidget::onEnableClickPublishToggled(bool enabled)
{
  Q_D(qSlicerPointSubscriberModuleWidget);
  
  if (enabled)
  {
    setupClickInteractor();
    d->StatusLabel->setText("Status: Click in 3D view to publish point");
  }
  else
  {
    cleanupClickInteractor();
    d->StatusLabel->setText("Status: Click publishing disabled");
  }
}

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberModuleWidget::setupClickInteractor()
{
  // Get the 3D view's interactor
  qMRMLThreeDView* threeDView = qSlicerApplication::application()
    ->layoutManager()->threeDWidget(0)->threeDView();
  
  if (!threeDView)
  {
    qWarning() << "Could not get 3D view";
    return;
  }
  
  vtkRenderWindowInteractor* interactor = threeDView->interactor();
  if (!interactor)
  {
    qWarning() << "Could not get interactor";
    return;
  }
  
  // Create and add the click callback
  ClickCallback* callback = ClickCallback::New();
  callback->Widget = this;
  
  this->ClickObserver = callback;
  this->ClickObserverTag = interactor->AddObserver(
    vtkCommand::LeftButtonPressEvent, callback);
}

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberModuleWidget::cleanupClickInteractor()
{
  if (this->ClickObserver && this->ClickObserverTag != 0)
  {
    qMRMLThreeDView* threeDView = qSlicerApplication::application()
      ->layoutManager()->threeDWidget(0)->threeDView();
    
    if (threeDView && threeDView->interactor())
    {
      threeDView->interactor()->RemoveObserver(this->ClickObserverTag);
    }
    
    this->ClickObserver->Delete();
    this->ClickObserver = nullptr;
    this->ClickObserverTag = 0;
  }
}

//-----------------------------------------------------------------------------
void qSlicerPointSubscriberModuleWidget::handleClick()
{
  Q_D(qSlicerPointSubscriberModuleWidget);
  
  // Get the 3D view
  qMRMLThreeDView* threeDView = qSlicerApplication::application()
    ->layoutManager()->threeDWidget(0)->threeDView();
  
  if (!threeDView)
    return;
  
  vtkRenderWindowInteractor* interactor = threeDView->interactor();
  if (!interactor)
    return;
  
  // Get the click position
  int* clickPos = interactor->GetEventPosition();
  
  // Use a world point picker to get 3D coordinates
  vtkNew<vtkWorldPointPicker> picker;
  picker->Pick(clickPos[0], clickPos[1], 0, threeDView->renderer());
  
  double* worldPos = picker->GetPickPosition();
  
  std::cout << "Clicked at 3D position: [" << worldPos[0] << ", " 
            << worldPos[1] << ", " << worldPos[2] << "]" << std::endl;
  
  // Publish the clicked point
  vtkSlicerPointSubscriberLogic* logic = d->logic();
  if (logic)
  {
    logic->PublishPoint(worldPos);
    d->StatusLabel->setText(QString("Published: [%1, %2, %3]")
      .arg(worldPos[0]).arg(worldPos[1]).arg(worldPos[2]));
  }
}