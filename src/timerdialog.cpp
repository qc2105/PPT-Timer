#include "timerdialog.h"
#include "ui_timerdialog.h"
#include <QMouseEvent>
#include <QDebug>
#include "settingdialog.h"
#include <QMessageBox>
#include "warningbox.h"

TimerDialog::TimerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimerDialog),
    time(0,0,0,0)
{
    ui->setupUi(this);
    setWindowTitle("Su's Timer");
    setWindowFlags(Qt::Window);

    warning = new WarningBox();
    setWindowFlag(Qt::Window);
    warning->setWindowTitle("Warning");

    this->isMoving = false;
    this->settingDialog = new SettingDialog(parent);
    this->timer = new QTimer(parent);
    this->timer->setInterval(1000);

    this->time = this->settingDialog->getTotalTime();
    this->ui->time_display->setText(this->time.toString("mm:ss"));

    connect(ui->start_button, &QPushButton::clicked, [this](){this->timer->start();});
    connect(ui->pause_button, &QPushButton::clicked, [this](){this->timer->stop();});
    connect(ui->setting_button, &QPushButton::clicked, [this](){this->settingDialog->show();});
    connect(ui->close_button, &QPushButton::clicked, [](){QApplication::quit();});
    connect(ui->refresh_button, &QPushButton::clicked, [this](){this->refresh();});
    connect(settingDialog, &SettingDialog::set, [this](){this->refresh();});

    connect(timer, &QTimer::timeout, [this](){
        this->time = this->time.addSecs(-1);
        if(this->time == this->settingDialog->getWarningTime())
        {
            warning->warning_display.setText(tr("Time Left:") + this->time.toString("mm:ss"));
            warning->resize(this->width()/2, this->height()/2);
            qDebug() << "warning size: " << warning->size() << endl;

            warning->show();
        }
        else if(this->time == this->settingDialog->getWarningTime().addSecs(-3))
        {
            warning->close();
        }
        else if(this->time == QTime(0, 0))
        {
            this->timer->stop();
            warning->warning_display.setText(tr("End"));
            warning->resize(this->width()/2, this->height()/2);
            qDebug() << "warning size: " << warning->size() << endl;

            warning->show();
        }
        this->ui->time_display->setText(this->time.toString("mm:ss"));
        warning->warning_display.setText(tr("Time Left:") + this->time.toString("mm:ss"));
    });
}

TimerDialog::~TimerDialog()
{
    delete ui;
}

void TimerDialog::mousePressEvent(QMouseEvent *event)
{
    this->dPos = event->pos();//鼠标相对窗口的坐标
    this->isMoving = true;
}

void TimerDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(this->isMoving)
    {
        QPoint mousePos = event->globalPos();//鼠标相对显示器的坐标
        this->move(mousePos - this->dPos);   //移动窗口
    }
}

void TimerDialog::mouseReleaseEvent(QMouseEvent* event)
{
    this->isMoving = false;
}

void TimerDialog::refresh()
{
    this->time = this->settingDialog->getTotalTime();
    this->ui->time_display->setText(this->time.toString("mm:ss"));
    this->timer->stop();
}


