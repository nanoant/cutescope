//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "ScopeView.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <libm2k/m2k.hpp>

QT_USE_NAMESPACE

ScopeView::ScopeView(QWidget* parent)
    : QChartView(parent), m_chart(new QChart), m_series(new QLineSeries(this)) {
  setChart(m_chart);
  setMinimumSize(800, 600);
  m_chart->setTheme(QChart::ChartTheme::ChartThemeDark);
  m_chart->addSeries(m_series);
  QValueAxis* axisX = new QValueAxis;
  axisX->setRange(0, 1024);
  axisX->setLabelFormat("%g");
  axisX->setTitleText("Samples");
  QValueAxis* axisY = new QValueAxis;
  axisY->setRange(-1, 1);
  axisY->setTitleText("Voltage");
  m_chart->addAxis(axisX, Qt::AlignBottom);
  m_series->attachAxis(axisX);
  m_chart->addAxis(axisY, Qt::AlignLeft);
  m_series->attachAxis(axisY);
  m_chart->legend()->hide();
  m_chart->setTitle("Oscilloscope");
}

ScopeView::~ScopeView() {}
