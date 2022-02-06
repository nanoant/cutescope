//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "SpinBox.h"

#include <cmath>

double const defVdiv[] = { 0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2,
                           0.5,   1.0,   2.0,   5.0,  10.0, 20.0, 50.0 };
const int nbVdiv = sizeof(defVdiv) / sizeof(*defVdiv);

double const defmsdiv[] = { 0.00001, 0.00002, 0.00005, 0.0001, 0.0002, 0.0005,
                            0.001,   0.002,   0.005,   0.01,   0.02,   0.05,
                            0.1,     0.2,     0.5,     1.0,    2.0,    5.0,
                            10.0,    20.0,    50.0,    100.0,  200.0 };
const int nbmsdiv = sizeof(defmsdiv) / sizeof(*defmsdiv);

SpinBox::SpinBox(SpinBox::Type type, QWidget* parent)
    : QDoubleSpinBox(parent), type(type) {
  setAlignment(Qt::AlignRight);
  setDecimals(3);
  switch (type) {
    case Type::msdiv:
      setSuffix(" ms");
      break;
    default:
      setSuffix(" V");
  }
}

void SpinBox::stepBy(int steps) {
  double v = value();
  switch (type) {
    case Type::Vdiv:
      if (steps > 0) {
        for (int i = 0; i < nbVdiv; i++) {
          if (v < defVdiv[i]) {
            v = defVdiv[i];
            break;
          }
        }
      } else if (steps < 0) {
        for (int i = nbVdiv - 1; i >= 0; i--) {
          if (v > defVdiv[i]) {
            v = defVdiv[i];
            break;
          }
        }
      }
      v = std::min(std::max(v, defVdiv[0]), defVdiv[nbVdiv - 1]);
      break;
    case Type::msdiv:
      if (steps > 0) {
        for (int i = 0; i < nbmsdiv; i++) {
          if (v < defmsdiv[i]) {
            v = defmsdiv[i];
            break;
          }
        }
      } else if (steps < 0) {
        for (int i = nbmsdiv - 1; i >= 0; i--) {
          if (v > defmsdiv[i]) {
            v = defmsdiv[i];
            break;
          }
        }
      }
      v = std::min(std::max(v, defmsdiv[0]), defmsdiv[nbmsdiv - 1]);
      break;
    default:
      if (steps > 0) {
        if (v >= 5) {
          double cv = std::ceil(v);
          v = std::abs(v - cv) > 0.01 ? cv : cv + 1;
        } else if (v >= 1) {
          double cv = std::ceil(v * 2) * 0.5;
          v = std::abs(v - cv) > 0.01 ? cv : cv + 0.5;
        } else if (v >= 0.1) {
          double cv = std::ceil(v * 10) * 0.1;
          v = std::abs(v - cv) > 0.01 ? cv : cv + 0.1;
        } else {
          double cv = std::ceil(v * 100) * 0.01;
          v = std::abs(v - cv) > 0.01 ? cv : cv + 0.01;
        }
      } else if (steps < 0) {
        if (v > 5) {
          double fv = std::floor(v);
          v = std::abs(v - fv) > 0.01 ? fv : fv - 1;
        } else if (v > 1) {
          double fv = std::floor(v * 2) * 0.5;
          v = std::abs(v - fv) > 0.01 ? fv : fv - 0.5;
        } else if (v > 0.1) {
          double fv = std::floor(v * 10) * 0.1;
          v = std::abs(v - fv) > 0.01 ? fv : fv - 0.1;
        } else {
          double fv = std::floor(v * 100) * 0.01;
          v = std::abs(v - fv) > 0.01 ? fv : fv - 0.01;
        }
      }
      v = std::min(std::max(v, 0.01), 10.0);
  }
  setValue(v);
}
