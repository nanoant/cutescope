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

namespace libm2k {
namespace analog {
class M2kAnalogIn;
}
}  // namespace libm2k

using namespace libm2k::analog;

class ScopeView : public QChartView {
  Q_OBJECT

 public:
  explicit ScopeView(M2kAnalogIn* ain = nullptr, QWidget* parent = nullptr);
  ~ScopeView();

 public slots:
  void updateData();

 private:
  int m_nbChannels;
  QTimer* m_timer;
  QChart* m_chart;
  std::vector<QLineSeries*> m_series;
  std::vector<QVector<QPointF>> m_seriesBuffers;
  std::vector<std::vector<double>> m_m2kBuffer;
  M2kAnalogIn* m_ain;
};
