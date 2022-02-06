//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#pragma once

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtWidgets/QWidget>

#include <vector>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Scope;

class ScopeView : public QChartView {
  Q_OBJECT

 public:
  explicit ScopeView(Scope* scope, QWidget* parent = nullptr);

 private:
  Scope* m_scope;
  int m_nbChannels;
  QTimer* m_timer;
  QChart* m_chart;
  std::vector<QLineSeries*> m_series;
  std::vector<QVector<QPointF>> m_seriesBuffers;
};
