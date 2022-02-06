//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#pragma once

#define SETUP_TRIGGER 1
// #define RESET_ON_START 1
// #define ALWAYS_CALIBRATE 1
// #define TRY_AND_EXIT 1
// #define ENABLE_AIN_CHANNEL_1 1
#define DEMO_CHANNELS 2

#include <QtCore/QObject>

namespace libm2k {
namespace context {
class M2k;
}
namespace analog {
class M2kAnalogIn;
class M2kAnalogOut;
}  // namespace analog
class M2kHardwareTrigger;
}  // namespace libm2k

using namespace libm2k;
using namespace libm2k::analog;
using namespace libm2k::context;

class Scope : public QObject {
  Q_OBJECT

 public:
  explicit Scope(QObject* parent = nullptr);
  ~Scope();

  QString firmwareVersion() const;
  QString serialNumber() const;
  int nbChannels() const { return m_nbChannels; }
  bool isChannelEnabled(int c) const;
  void read(std::vector<QVector<QPointF>>& buffers);

  double msdiv() const;
  void setmsdiv(double value);

  bool channelEnabled(int c) const;
  void enableChannel(int c, bool enabled);
  double Vdiv(int c) const;
  void setVdiv(int c, double value);
  double Vpos(int c) const;
  void setVpos(int c, double value);

  bool triggerEnabled();
  void enableTrigger(bool enabled);
  int triggerEdge() const;
  void setTriggerEdge(int value);
  double triggerLevel() const;
  void setTriggerLevel(double value);
  double triggerHoldOffset() const;
  void setTriggerHoldOffset(double value);
  double triggerHorzPos() const;
  void setTriggerHorzPos(double value);

  double powerEnabled(int c) const;
  void enablePower(int c, bool enabled);
  double powerVout(int c) const;
  void setPowerVout(int c, double value);

 signals:
  void msdivChanged(double value);
  void VdivChanged(int c, double value);
  void VposChanged(int c, double value);

 private:
  M2k* const ctx;
  M2kAnalogIn* const ain;
  M2kAnalogOut* const aout;
  M2kHardwareTrigger* const trig;
  int const m_nbChannels;
  std::vector<std::vector<double>> m_m2kBuffer;
};
