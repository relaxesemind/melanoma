#include "animatedprogressbar.h"

AnimatedProgressBar::AnimatedProgressBar(QWidget *parent) : QProgressBar (parent)
{
    setHidden(false);
    setStyleSheet(QString(
                           "QProgressBar:horizontal {"
//                           "border: 1px solid gray;"
                           "border-radius: 3px;"
                           "background: white;"
                           "padding: 1px;"
                           "}"
                           "QProgressBar::chunk:horizontal {"
                           "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 #FF893D, stop: 1 white);"
                           "}"
    ));
}

void AnimatedProgressBar::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
