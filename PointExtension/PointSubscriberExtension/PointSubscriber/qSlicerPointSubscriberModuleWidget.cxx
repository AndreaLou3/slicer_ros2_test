// Qt includes
#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

// Slicer includes
#include "qSlicerPointSubscriberModuleWidget.h"
#include "ui_qSlicerPointSubscriberModuleWidget.h"
#include <qSlicerApplication.h>
#include <qSlicerLayoutManager.h>
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>
#include <qSlicerPythonManager.h>

// Logic includes
#include "vtkSlicerPointSubscriberLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLMarkupsFiducialNode.h>

// VTK includes
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkWorldPointPicker.h>
#include <vtkNew.h>

// ADDED: Custom callback class for handling clicks
class ClickCallback : public vtkCommand
{
public:
  static ClickCallback* New() { return new ClickCallback; }
  
  void Execute(vtkObject* /*caller*/, unsigned long eventId, void* /*callData*/) override
  {
    if (eventId == vtkCommand::LeftButtonPressEvent && this->Widget)
    {
      this->Widget->handleClick();
    }
  }
  
  qSlicerPointSubscriberModuleWidget* Widget = nullptr;
};

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPointSubscriberModuleWidgetPrivate
  : public Ui_qSlicerPointSubscriberModuleWidget
{
  Q_DECLARE_PUBLIC(qSlicerPointSubscriberModuleWidget);
protected:
  qSlicerPointSubscriberModuleWidget* const q_ptr;

public:
  qSlicerPointSubscriberModuleWidgetPrivate(
    qSlicerPointSubscriberModuleWidget& object);
  vtkSlicerPointSubscriberLogic* logic() const;
  
  // ADDED: UI elements
  QPushButton* StartSubscriberButton;
  QCheckBox* EnableClickPublishCheckbox;
  QLabel* StatusLabel;
};

//-----------------------------------------------------------------------------
// qSlicerPointSubscriberModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModuleWidgetPrivate
::qSlicerPointSubscriberModuleWidgetPrivate(
  qSlicerPointSubscriberModuleWidget& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
vtkSlicerPointSubscriberLogic*
qSlicerPointSubscriberModuleWidgetPrivate::logic() const
{
  Q_Q(const qSlicerPointSubscriberModuleWidget);
  return vtkSlicerPointSubscriberLogic::SafeDownCast(q->logic());
}

//-----------------------------------------------------------------------------
// qSlicerPointSubscriberModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModuleWidget::qSlicerPointSubscriberModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPointSubscriberModuleWidgetPrivate(*this) )
  , ClickObserver(nullptr)
  , ClickObserverTag(0)
{
}

//-----------------------------------------------------------------------------
qSlicerPointSubscriberModuleWidget::~qSlicerPointSubscriberModuleWidget()
{
  cleanupClickInteractor();
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
  
  // Auto-start the subscriber and publisher
  vtkSlicerPointSubscriberLogic* logic = d->logic();
  if (logic)
  {
    logic->InitializeSubscriber();
    logic->InitializePublisher();
    d->StatusLabel->setText("Status: Subscriber and Publisher initialized");
  }
}

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
  
  // Publish using Python
  QString pythonCode = QString(
    "import vtk\n"
    "scene = slicer.mrmlScene\n"
    "pubNode = scene.GetFirstNodeByName('ros2:pub:/clicked_point_topic')\n"
    "if pubNode:\n"
    "    arr = vtk.vtkDoubleArray()\n"
    "    arr.SetNumberOfValues(3)\n"
    "    arr.SetValue(0, %1)\n"
    "    arr.SetValue(1, %2)\n"
    "    arr.SetValue(2, %3)\n"
    "    pubNode.Publish(arr)\n"
    "    print('Published: [%1, %2, %3]')\n"
  ).arg(worldPos[0]).arg(worldPos[1]).arg(worldPos[2]);
  
  qSlicerApplication::application()->pythonManager()->executeString(pythonCode);
  
  d->StatusLabel->setText(QString("Published: [%1, %2, %3]")
    .arg(worldPos[0]).arg(worldPos[1]).arg(worldPos[2]));
}