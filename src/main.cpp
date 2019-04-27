#include "timerdialog.h"
#include <QApplication>
#include "ui_timerdialog.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimerDialog w;

    LabelStretcher stretch{&w};
    stretch.setManaged((w.ui->time_display));

    LabelStretcher stretch_warning{w.warning};
    stretch_warning.setManaged(&(w.warning->warning_display));

    w.show();

    return a.exec();
}
