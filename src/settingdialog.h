#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    QTime getTotalTime();
    QTime getWarningTime();

signals:
    void set(void);

private slots:
    void on_pushButtonCancel_clicked();

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
