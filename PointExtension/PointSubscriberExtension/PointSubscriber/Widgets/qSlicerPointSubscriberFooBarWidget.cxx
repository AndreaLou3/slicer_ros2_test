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
}

//-----------------------------------------------------------------------------
qSlicerPointSubscriberFooBarWidget
::~qSlicerPointSubscriberFooBarWidget()
{
}
