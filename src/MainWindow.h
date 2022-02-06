//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#pragma once

#include <QtWidgets/QMainWindow>

class Scope;
class ScopeView;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);

 private:
  Scope* m_scope;
  ScopeView* m_scopeView;
};
