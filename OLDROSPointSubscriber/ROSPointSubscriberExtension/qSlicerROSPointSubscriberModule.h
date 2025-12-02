#pragma once

#include "qSlicerLoadableModule.h"

class qSlicerROSPointSubscriberModule : public qSlicerLoadableModule
{
  Q_OBJECT
public:
  qSlicerROSPointSubscriberModule(QObject* parent=nullptr);
  ~qSlicerROSPointSubscriberModule() override = default;

  qSlicerGetTitleMacro("ROS Point Subscriber");
  QString helpText() const override { return "Minimal ROS2 PointStamped subscriber."; }
  QString acknowledgmentText() const override { return ""; }
  bool isHidden() const override { return false; }

protected:
  void setup() override;
};

