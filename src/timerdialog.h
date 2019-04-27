#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QTime>
#include "settingdialog.h"
#include <QMessageBox>
#include "warningbox.h"

namespace Ui {
class TimerDialog;
}

class TimerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimerDialog(QWidget *parent = nullptr);
    ~TimerDialog();

    Ui::TimerDialog *ui;
    WarningBox * warning;

public slots:
    void refresh();

private:
    QPoint dPos;
    bool isMoving;
    QTimer* timer;
    QTime time;
    SettingDialog* settingDialog;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};


#include <QtWidgets>

class LabelStretcher : public QObject {
   Q_OBJECT
   static constexpr const char kMinimumsAcquired[] = "ls_minimumsAcquired";
   static constexpr const char kStretcherManaged[] = "ls_stretcherManaged";
public:
   LabelStretcher(QObject *parent = nullptr) : QObject(parent) {
      apply(qobject_cast<QWidget*>(parent));
   }
   void apply(QWidget *widget) {
      if (!widget)
      {
          qDebug() << "Return because widget == null";
          return;
      }
      setManaged(widget);
      setMinimumSize(widget);
      widget->installEventFilter(this);
   }
   void setManaged(QWidget *w, bool managed = true) {
      w->setProperty(kStretcherManaged, managed);
   }
protected:
   bool eventFilter(QObject * obj, QEvent * ev) override {
      auto widget = qobject_cast<QWidget*>(obj);
      if (widget && ev->type() == QEvent::Resize)
         resized(widget);
      return false;
   }
private:
   void onLayout(QLayout *layout, const std::function<void(QWidget*)> &onWidget) {
      if (!layout) return;
      auto N = layout->count();
      for (int i = 0; i < N; ++i) {
         auto item = layout->itemAt(i);
         onWidget(item->widget());
         onLayout(item->layout(), onWidget);
      }
   }
   void setFont(QLayout *layout, const QFont &font) {
      onLayout(layout, [&](QWidget *widget){ setFont(widget, font); });
   }
   void setFont(QWidget *widget, const QFont &font) {
      if (!widget || !widget->property(kStretcherManaged).toBool()) return;
      widget->setFont(font);
      setFont(widget->layout(), font);
   }
   void setMinimumSize(QWidget *widget) {
      if (widget->layout()) return;
      widget->setMinimumSize(widget->minimumSizeHint());
   }
   static int dSize(const QSizeF & inner, const QSizeF & outer) {
      auto dy = inner.height() - outer.height();
      auto dx = inner.width() - outer.width();
      return std::max(dx, dy);
   }
   qreal f(qreal fontSize, QWidget *widget) {
      auto font = widget->font();
      font.setPointSizeF(fontSize);
      setFont(widget, font);
      auto d = dSize(widget->sizeHint(), widget->size());
      qDebug() << "f:" << fontSize << "d" << d;
      return d;
   }
   qreal df(qreal fontSize, qreal dStep, QWidget *widget) {
      fontSize = std::max(dStep + 1.0, fontSize);
      return (f(fontSize + dStep, widget) - f(fontSize - dStep, widget)) / dStep;
   }
   void resized(QWidget *widget) {
      qDebug() << "pre: " << widget->minimumSizeHint() << widget->sizeHint() << widget->size();
      if (!widget->property(kMinimumsAcquired).toBool()) {
         onLayout(widget->layout(), [=](QWidget *widget){ setMinimumSize(widget); });
         widget->setProperty(kMinimumsAcquired, true);
      }

       // Newton's method
      auto font = widget->font();
      auto fontSize = font.pointSizeF();
      qreal dStep = 1.0;
      int i;
      for (i = 0; i < 10; ++i) {
         auto prevFontSize = fontSize;
         auto d = df(fontSize, dStep, widget);
         if (d == 0) {
            dStep *= 2.0;
            continue;
         }
         fontSize -= f(fontSize, widget)/d;
         fontSize = std::max(dStep + 1.0, fontSize);
         auto change = fabs(prevFontSize - fontSize)/fontSize;
         qDebug() << "d:" << d << " delta" << change;
         if (change < 0.01) break; // we're within 1% of target
      }
      font.setPointSizeF(fontSize);
      setFont(widget, font);
      qDebug() << "post:" << i << widget->minimumSizeHint() << widget->sizeHint() << widget->size();
   }
};
constexpr const char LabelStretcher::kMinimumsAcquired[];
constexpr const char LabelStretcher::kStretcherManaged[];


#endif // TIMERDIALOG_H
