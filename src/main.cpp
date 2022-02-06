//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "MainWindow.h"

#include <QtWidgets/QApplication>

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}
