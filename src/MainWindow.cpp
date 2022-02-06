//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "MainWindow.h"
#include "PropertiesWidget.h"
#include "Scope.h"
#include "ScopeView.h"

#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QWidget>

QT_USE_NAMESPACE

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_scope(new Scope(this)),
      m_scopeView(new ScopeView(m_scope, this)) {
  QDockWidget* dockWidget = new QDockWidget(this);
  dockWidget->setStyleSheet(
      "QDockWidget {"
      "  titlebar-close-icon: none;"
      "  titlebar-normal-icon: none;"
      "}");
  dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  dockWidget->setWidget(new PropertiesWidget(m_scope, this));
  dockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                          QDockWidget::DockWidgetFloatable);
  addDockWidget(Qt::RightDockWidgetArea, dockWidget);
  setCentralWidget(m_scopeView);
  resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
  setWindowTitle("Cutescope [m2k " + m_scope->firmwareVersion() + "]");
}
