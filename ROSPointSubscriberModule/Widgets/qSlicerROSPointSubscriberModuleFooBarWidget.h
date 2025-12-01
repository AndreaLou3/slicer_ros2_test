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

#ifndef __qSlicerROSPointSubscriberModuleFooBarWidget_h
#define __qSlicerROSPointSubscriberModuleFooBarWidget_h

// Qt includes
#include <QWidget>

// FooBar Widgets includes
#include "qSlicerROSPointSubscriberModuleModuleWidgetsExport.h"

class qSlicerROSPointSubscriberModuleFooBarWidgetPrivate;

class Q_SLICER_MODULE_ROSPOINTSUBSCRIBERMODULE_WIDGETS_EXPORT qSlicerROSPointSubscriberModuleFooBarWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qSlicerROSPointSubscriberModuleFooBarWidget(QWidget *parent=0);
  ~qSlicerROSPointSubscriberModuleFooBarWidget() override;

protected slots:

protected:
  QScopedPointer<qSlicerROSPointSubscriberModuleFooBarWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerROSPointSubscriberModuleFooBarWidget);
  Q_DISABLE_COPY(qSlicerROSPointSubscriberModuleFooBarWidget);
};

#endif
