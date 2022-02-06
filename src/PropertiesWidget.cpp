//
// Copyright (c) 2022 Adam Strzelecki, All rights reserved.
//
// Released under GPL v3 license.
//

#include "PropertiesWidget.h"
#include "Scope.h"
#include "SpinBox.h"

#include "colors.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_USE_NAMESPACE

PropertiesWidget::PropertiesWidget(Scope* scope, QWidget* parent)
    : QWidget(parent), m_scope(scope) {
  QVBoxLayout* vboxLayout = new QVBoxLayout(this);
  QGridLayout* gridLayout = new QGridLayout();

  int row = 0;
  QLabel* label;
  QCheckBox* checkBox;
  QComboBox* comboBox;
  SpinBox* spinBox;

  gridLayout->addWidget(new QLabel(tr("ms / Div.")), row, 0, Qt::AlignRight);
  gridLayout->addWidget(spinBox = new SpinBox(SpinBox::Type::msdiv), row++, 1);
  spinBox->setValue(m_scope->msdiv());
  connect(spinBox,
          QOverload<double>::of(&SpinBox::valueChanged),
          [=](double value) { m_scope->setmsdiv(value); });

  QString sectionStyle =
      "border: 1px solid #88888888; padding: 10px 2px 2px 2px; border-top: 0; "
      "border-left: 0; border-right: 0;";

  for (int c = 0; c < 2; c++) {
    gridLayout->addWidget(
        checkBox = new QCheckBox(tr("Channel %1").arg(c + 1)), row++, 0, 1, 0);
    checkBox->setStyleSheet(
        QString("QCheckBox { color: %1; %2 border-color: %1; } "
                "QCheckBox::indicator { accent-color: %1; }")
            .arg(colors::channel[c % colors::nbChannel])
            .arg(sectionStyle));
    checkBox->setChecked(m_scope->channelEnabled(c));
    connect(checkBox, &QCheckBox::stateChanged, [=](int value) {
      m_scope->enableChannel(c, value != 0);
    });

    gridLayout->addWidget(new QLabel(tr("V / Div.")), row, 0, Qt::AlignRight);
    gridLayout->addWidget(spinBox = new SpinBox(SpinBox::Type::Vdiv), row++, 1);
    spinBox->setValue(m_scope->Vdiv(c));
    connect(spinBox,
            QOverload<double>::of(&SpinBox::valueChanged),
            [=](double value) { m_scope->setVdiv(c, value); });

    gridLayout->addWidget(new QLabel(tr("V Pos.")), row, 0, Qt::AlignRight);
    gridLayout->addWidget(spinBox = new SpinBox(), row++, 1);
    spinBox->setValue(m_scope->Vpos(c));
    connect(spinBox,
            QOverload<double>::of(&SpinBox::valueChanged),
            [=](double value) { m_scope->setVpos(c, value); });
  }

  gridLayout->addWidget(
      checkBox = new QCheckBox(tr("Trigger")), row++, 0, 1, 0);
  checkBox->setStyleSheet(sectionStyle);
  checkBox->setChecked(m_scope->triggerEnabled());
  connect(checkBox, &QCheckBox::stateChanged, [=](int value) {
    m_scope->enableTrigger(value != 0);
  });

  gridLayout->addWidget(new QLabel(tr("Edge")), row, 0, Qt::AlignRight);
  gridLayout->addWidget(comboBox = new QComboBox(), row++, 1);
  comboBox->setCurrentIndex(m_scope->triggerEdge());
  connect(comboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged),
          [=](int value) { m_scope->setTriggerEdge(value); });
  comboBox->addItem(tr("Rising") + " [+]");
  comboBox->addItem(tr("Falling") + " [-]");
  gridLayout->addWidget(new QLabel(tr("Level")), row, 0, Qt::AlignRight);
  gridLayout->addWidget(spinBox = new SpinBox(), row++, 1);
  spinBox->setValue(m_scope->triggerLevel());
  connect(spinBox,
          QOverload<double>::of(&SpinBox::valueChanged),
          [=](double value) { m_scope->setTriggerLevel(value); });

  gridLayout->addWidget(new QLabel(tr("Hold Off.")), row, 0, Qt::AlignRight);
  gridLayout->addWidget(spinBox = new SpinBox(), row++, 1);
  spinBox->setValue(m_scope->triggerHoldOffset());
  connect(spinBox,
          QOverload<double>::of(&SpinBox::valueChanged),
          [=](double value) { m_scope->setTriggerHoldOffset(value); });

  gridLayout->addWidget(new QLabel(tr("Horz. Pos.")), row, 0, Qt::AlignRight);
  gridLayout->addWidget(spinBox = new SpinBox(), row++, 1);
  spinBox->setValue(m_scope->triggerHorzPos());
  connect(spinBox,
          QOverload<double>::of(&SpinBox::valueChanged),
          [=](double value) { m_scope->setTriggerHorzPos(value); });

  gridLayout->addWidget(label = new QLabel(tr("Power Supply")), row++, 0, 1, 0);
  label->setStyleSheet(sectionStyle);

  for (int c = 0; c < 2; c++) {
    gridLayout->addWidget(
        checkBox = new QCheckBox(c == 0 ? "V+" : "V-"), row, 0);
    checkBox->setStyleSheet(
        QString("color: %1;").arg(colors::channel[c % colors::nbChannel]));
    checkBox->setChecked(m_scope->powerEnabled(c));
    connect(checkBox, &QCheckBox::stateChanged, [=](int value) {
      m_scope->enablePower(c, value != 0);
    });

    gridLayout->addWidget(spinBox = new SpinBox(), row++, 1);
    spinBox->setValue(m_scope->powerVout(c));
    connect(spinBox,
            QOverload<double>::of(&SpinBox::valueChanged),
            [=](double value) { m_scope->setPowerVout(c, value); });
  }

  vboxLayout->addLayout(gridLayout);
  vboxLayout->addStretch();
}
