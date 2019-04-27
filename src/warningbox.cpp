#include "warningbox.h"

WarningBox::WarningBox(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("color: rgb(255, 0, 0);background-color: rgb(135, 206, 250);");
    this->setLayout(new QGridLayout);
    this->layout()->addWidget(&warning_display);
    this->layout()->setAlignment(Qt::AlignCenter);

    warning_display.setAlignment(Qt::AlignCenter);
}

