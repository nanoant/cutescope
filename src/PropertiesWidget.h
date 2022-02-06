//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#pragma once

#include "ScopeView.h"

class Scope;

class PropertiesWidget : public QWidget {
  Q_OBJECT

 public:
  explicit PropertiesWidget(Scope* scope, QWidget* parent = nullptr);

 private:
  Scope* m_scope;
};
