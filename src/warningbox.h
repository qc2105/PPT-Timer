#ifndef WARNINGBOX_H
#define WARNINGBOX_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

class WarningBox : public QWidget
{
    Q_OBJECT
public:
    explicit WarningBox(QWidget *parent = nullptr);
    QLabel warning_display;

signals:

public slots:
};

#endif // WARNINGBOX_H
