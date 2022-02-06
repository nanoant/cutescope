//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#pragma once

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

class SpinBox : public QDoubleSpinBox {
  Q_OBJECT

 public:
  enum struct Type { V, Vdiv, msdiv };
  explicit SpinBox(Type type = Type::V, QWidget* parent = nullptr);

  Type const type;

 protected:
  void stepBy(int steps) override;
};
