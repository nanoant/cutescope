//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "Scope.h"

#include <QtWidgets/QMessageBox>

#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/analog/m2kanalogout.hpp>
#include <libm2k/analog/m2kpowersupply.hpp>
#include <libm2k/contextbuilder.hpp>
#include <libm2k/m2k.hpp>

#include <iterator>
#include <ostream>
#include "libm2k/analog/enums.hpp"

QT_USE_NAMESPACE

Scope::Scope(QObject* parent)
    : QObject(parent),
      ctx(m2kOpen()),
      ain(ctx ? ctx->getAnalogIn() : nullptr),
      aout(ctx ? ctx->getAnalogOut() : nullptr),
      trig(ctx ? ain->getTrigger() : nullptr),
      m_nbChannels(ain ? ain->getNbChannels() : DEMO_CHANNELS) {

  if (!ctx) {
    return;
  }

  std::cerr << "m2k " << ctx->getFirmwareVersion() << " "
            << "SN " << ctx->getSerialNumber() << " "
            << "found." << std::endl;

#if RESET_ON_START
  // NOTE: This will invalidate all calibrations.
  ain->reset();
  aout->reset();
#endif

#if ALWAYS_CALIBRATE
  std::cerr << "Calibrating..." << std::endl;
  ctx->calibrateADC();
  ctx->calibrateDAC();
  std::cerr << "Calibrating. Done." << std::endl;
#else
  if (!ctx->isCalibrated()) {
    QMessageBox msgBox;
    msgBox.setText(
        "m2k is not calibrated. Disconnect all inputs are press OK.");
    msgBox.exec();
    std::cerr << "Calibrating..." << std::endl;
    ctx->calibrateADC();
    ctx->calibrateDAC();
    std::cerr << "Calibrating. Done." << std::endl;
  } else {
    std::cerr << "Already calibrated." << std::endl;
  }
#endif

  ain->setSampleRate(100000);
  ain->enableChannel(0, true);
  ain->setRange((ANALOG_IN_CHANNEL)0, PLUS_MINUS_2_5V);
#if ENABLE_AIN_CHANNEL_1
  ain->enableChannel(1, true);
  ain->setRange((ANALOG_IN_CHANNEL)1, PLUS_MINUS_25V);
#endif

#ifdef SETUP_TRIGGER
  trig->setAnalogSource(CHANNEL_1);
  trig->setAnalogCondition(0, RISING_EDGE_ANALOG);
  trig->setAnalogLevel(0, 0);
  trig->setAnalogDelay(0);
  trig->setAnalogMode(0, ANALOG);
#endif

  aout->setSampleRate(0, 750000);
  aout->setSampleRate(1, 750000);
  aout->enableChannel(0, true);
  aout->enableChannel(1, true);

  std::vector<double> sinWaveform;
  std::vector<double> sawWaveform;

  for (int i = 0; i < 1024; i++) {
    double rad = 2 * M_PI * (i / 1024.0);
    double val = std::sin(rad);
    sinWaveform.push_back(val);
    sawWaveform.push_back((2 * i) / 1024.0);
  }

  aout->setCyclic(true);
  aout->push(0, sinWaveform);
  aout->push(1, sawWaveform);
}

Scope::~Scope() {
  if (aout) {
    aout->stop();
  }
  if (ctx) {
    contextClose(ctx);
  }
}

QString Scope::firmwareVersion() const {
  return ctx ? QString::fromStdString(ctx->getFirmwareVersion()) : tr("DEMO");
}

QString Scope::serialNumber() const {
  return ctx ? QString::fromStdString(ctx->getSerialNumber()) : tr("DEMO");
}

bool Scope::isChannelEnabled(int c) const {
  return ain ? ain->isChannelEnabled(c) : true;
}

void Scope::read(std::vector<QVector<QPointF>>& buffers) {
  for (int c = 0; c < m_nbChannels; c++) {
    if (buffers[c].isEmpty()) {
      for (int i = 0; i < 1024; i++) {
        double mspt = 1000.0 / 100000;
        double half = 1024 / 2.0 * mspt;
        buffers[c].append(QPointF(i * mspt - half, 0));
      }
    }
  }
  if (ain) {
    ain->getSamples(m_m2kBuffer, 1024);
  } else {
    if (m_m2kBuffer.empty()) {
      for (int c = 0; c < m_nbChannels; c++) {
        m_m2kBuffer.emplace_back(1024, 0.0);
      }
    }
    for (int c = 0; c < m_nbChannels; c++) {
      for (int i = 0; i < 1024; i++) {
        double rad = 2 * M_PI * (i / 1024.0);
        double val = std::sin(rad);
        m_m2kBuffer[c][i] = val + (std::rand() / (double)RAND_MAX - 0.5) / 50.0;
      }
    }
  }
  int ec = 0;
  for (int c = 0; c < m_nbChannels; c++) {
    if (ain && ain->isChannelEnabled(c) || !ain) {
      for (int i = 0; i < 1024; i++) {
        buffers[c][i].setY(m_m2kBuffer[ec][i]);
      }
      ec++;
    } else {
      // Fill data with pure sin template
      for (int i = 0; i < 1024; i++) {
        double rad = 2 * M_PI * (i / 1024.0) * (750000.0 / 100000);
        double val = std::sin(rad);
        buffers[c][i].setY(val);
      }
    }
  }
}

bool Scope::channelEnabled(int c) const {
  return ain ? ain->isChannelEnabled(c) : true;
}

void Scope::enableChannel(int c, bool enabled) {
  if (ain) {
    ain->enableChannel(c, enabled);
  }
}

double Scope::msdiv() const {
  return ain ? ain->getSampleRate() / 100000 : 1.0;
}

void Scope::setmsdiv(double value) {
  std::cerr << "ms/div: " << value << std::endl;
  msdivChanged(value);
  if (ain) {
    ain->setSampleRate(value * 100000);
  }
}

double Scope::Vdiv(int c) const { return 1.0; }

void Scope::setVdiv(int c, double value) {
  std::cerr << "V/div [" << c << "]: " << value << std::endl;
  VdivChanged(c, value);
}

double Scope::Vpos(int c) const { return 0.0; }

void Scope::setVpos(int c, double value) {}

bool Scope::triggerEnabled() { return true; }

void Scope::enableTrigger(bool enabled) {}

int Scope::triggerEdge() const {
  return trig ? trig->getAnalogCondition(0) : 0;
}

void Scope::setTriggerEdge(int value) {}

double Scope::triggerLevel() const {
  return trig ? trig->getAnalogLevel(0) : 0.5;
}

void Scope::setTriggerLevel(double value) {
  if (trig) {
    trig->setAnalogLevel(0, value);
  }
}

double Scope::triggerHoldOffset() const { return 0.0; }

void Scope::setTriggerHoldOffset(double value) {}

double Scope::triggerHorzPos() const { return 0.0; }

void Scope::setTriggerHorzPos(double value) {}

double Scope::powerEnabled(int c) const { return false; }

void Scope::enablePower(int c, bool enabled) {}

double Scope::powerVout(int c) const { return 3.3; }

void Scope::setPowerVout(int c, double value) {}
