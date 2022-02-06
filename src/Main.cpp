//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "ScopeView.h"
#include "libm2k/enums.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <iterator>
#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/analog/m2kanalogout.hpp>
#include <libm2k/analog/m2kpowersupply.hpp>
#include <libm2k/contextbuilder.hpp>
#include <libm2k/m2k.hpp>
#include <ostream>

using namespace libm2k;
using namespace libm2k::analog;
using namespace libm2k::context;

#define SETUP_TRIGGER 1
// #define RESET_ON_START 1
// #define ALWAYS_CALIBRATE 1
// #define TRY_AND_EXIT 1
// #define ENABLE_AIN_CHANNEL_1 1

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  M2k* ctx = m2kOpen();

  if (ctx) {

    std::cerr << "m2k " << ctx->getFirmwareVersion() << " "
              << "SN " << ctx->getSerialNumber() << " "
              << "found." << std::endl;

    M2kAnalogIn* ain = ctx->getAnalogIn();
    M2kAnalogOut* aout = ctx->getAnalogOut();
#ifdef SETUP_TRIGGER
    M2kHardwareTrigger* trig = ain->getTrigger();
#endif

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
    ain->setRange((ANALOG_IN_CHANNEL)0, -2.0, 2.0);
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

#if TRY_AND_EXIT
    // Just try to get samples and exit
    int ret = 0;
    auto buffer = ain->getSamples(1024);
#else
    ScopeView scope(ain);
    std::string title = "Cutescope [m2k " + ctx->getFirmwareVersion() + "]";
    scope.setWindowTitle(QString::fromUtf8(title.c_str()));
    scope.show();
    int ret = app.exec();
#endif

    aout->stop();
    contextClose(ctx);

    std::cerr << "Shutdown complete." << std::endl;

    return ret;

  } else {

    std::cerr << "No m2k found, running in demo mode." << std::endl;

    ScopeView scope;
    std::string title = "Cutescope [demo mode]";
    scope.setWindowTitle(QString::fromUtf8(title.c_str()));
    scope.show();
    return app.exec();
  }
}
