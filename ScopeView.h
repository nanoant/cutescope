//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#pragma once

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtWidgets/QWidget>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ScopeView : public QChartView {
  Q_OBJECT

 public:
  explicit ScopeView(QWidget* parent = nullptr);
  ~ScopeView();

 private:
  QChart* m_chart;
  QLineSeries* m_series;
};
