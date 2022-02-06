//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "ScopeView.h"
#include "Scope.h"

#include "colors.h"

#include <QtCore/QTimer>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_USE_NAMESPACE

// #define USE_OPENGL 1
// #define TARGET_FPS 30
#define TARGET_FPS 60

ScopeView::ScopeView(Scope* scope, QWidget* parent)
    : QChartView(parent),
      m_scope(scope),
      m_timer(new QTimer),
      m_chart(new QChart),
      m_series(),
      m_nbChannels(scope->nbChannels()) {
#if USE_OPENGL
  m_series->setUseOpenGL(true);
#endif
  setFocusPolicy(Qt::FocusPolicy::ClickFocus);
  setChart(m_chart);
  setMinimumSize(640, 480);
  m_chart->setTheme(QChart::ChartTheme::ChartThemeDark);
  for (int c = 0; c < m_nbChannels; c++) {
    QLineSeries* series = new QLineSeries;
    m_series.push_back(series);
    m_chart->addSeries(series);
    m_seriesBuffers.emplace_back();
  }
  QValueAxis* axisX = new QValueAxis;
  double mspt = 1000.0 / 100000;
  double half = 1024 / 2.0 * mspt;
  axisX->setRange(-2, 2);
  axisX->setLabelFormat("%g");
  axisX->setTitleText("ms");
  QValueAxis* axisY = new QValueAxis;
  axisY->setRange(-2, 2);
  axisY->setTitleText("V");
  m_chart->addAxis(axisX, Qt::AlignBottom);
  m_chart->addAxis(axisY, Qt::AlignLeft);
  for (int c = 0; c < m_nbChannels; c++) {
    bool enabled = m_scope->isChannelEnabled(c);
    QPen pen;
    QColor color = colors::channel[c % colors::nbChannel];
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
  connect(m_scope, &Scope::msdivChanged, [=](double value) {
    axisX->setRange(-2 * value, 2 * value);
  });
  connect(m_scope, &Scope::VdivChanged, [=](int c, double value) {
    if (c == 0) {
      axisY->setRange(-2 * value, 2 * value);
    }
  });
  connect(m_timer, &QTimer::timeout, this, [=]() {
    m_scope->read(m_seriesBuffers);
    for (int c = 0; c < m_nbChannels; c++) {
      m_series[c]->replace(m_seriesBuffers[c]);
    }
  });
  m_timer->setInterval((int)(1000.0 / TARGET_FPS));
  m_timer->start();
}
