//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "ScopeView.h"

#include <QtCore/QTimer>

#include <QtWidgets/QDesktopWidget>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <libm2k/analog/m2kanalogin.hpp>
#include <libm2k/m2k.hpp>

QT_USE_NAMESPACE

using namespace libm2k::analog;

// #define USE_OPENGL 1
// #define TARGET_FPS 30
#define TARGET_FPS 60

ScopeView::ScopeView(M2kAnalogIn* ain, QWidget* parent)
    : QChartView(parent),
      m_ain(ain),
      m_timer(new QTimer),
      m_chart(new QChart),
      m_series(),
      m_nbChannels(ain ? ain->getNbChannels() : 2) {
#if USE_OPENGL
  m_series->setUseOpenGL(true);
#endif
  setChart(m_chart);
  setMinimumSize(640, 480);
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
  m_chart->setTheme(QChart::ChartTheme::ChartThemeDark);
  for (int c = 0; c < m_nbChannels; c++) {
    QLineSeries* series = new QLineSeries;
    m_series.push_back(series);
    m_chart->addSeries(series);
    m_seriesBuffers.emplace_back();
  }
  QValueAxis* axisX = new QValueAxis;
  axisX->setRange(0, 1024);
  axisX->setLabelFormat("%g");
  axisX->setTitleText("t");
  QValueAxis* axisY = new QValueAxis;
  axisY->setRange(-2, 2);
  axisY->setTitleText("V");
  m_chart->addAxis(axisX, Qt::AlignBottom);
  m_chart->addAxis(axisY, Qt::AlignLeft);
  for (int c = 0; c < m_nbChannels; c++) {
    bool enabled = !m_ain || m_ain->isChannelEnabled(c);
    QPen pen;
    QColor color = c == 0   ? QColor(255, 96, 96)  // probe red
                   : c == 1 ? QColor(0, 192, 192)  // probe cyan
                   : c == 2 ? QColor(0, 255, 0)
                   : c == 3 ? QColor(0, 0, 255)
                            : QColor(128, 128, 128);
    if (!enabled) {
      color.setAlpha(16);
    }
    pen.setColor(color);
    QLineSeries* series = m_series[c];
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    series->setPen(pen);
  }
  m_chart->legend()->hide();
  // m_chart->setTitle("Oscilloscope");
  connect(m_timer, &QTimer::timeout, this, &ScopeView::updateData);
  m_timer->setInterval((int)(1000.0 / TARGET_FPS));
  m_timer->start();
}

void ScopeView::updateData() {
  for (int c = 0; c < m_nbChannels; c++) {
    if (m_seriesBuffers[c].isEmpty()) {
      for (int i = 0; i < 1024; i++) {
        m_seriesBuffers[c].append(QPointF(i, 0));
      }
    }
  }
  if (m_ain) {
    m_ain->getSamples(m_m2kBuffer, 1024);
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
    if (m_ain && m_ain->isChannelEnabled(c) || !m_ain) {
      for (int i = 0; i < 1024; i++) {
        m_seriesBuffers[c][i].setY(m_m2kBuffer[ec][i]);
      }
      ec++;
    } else {
      // Fill data with pure sin template
      for (int i = 0; i < 1024; i++) {
        double rad = 2 * M_PI * (i / 1024.0) * (750000.0 / 100000);
        double val = std::sin(rad);
        m_seriesBuffers[c][i].setY(val);
      }
    }
  }
  for (int c = 0; c < m_nbChannels; c++) {
    m_series[c]->replace(m_seriesBuffers[c]);
  }
}

ScopeView::~ScopeView() {}
