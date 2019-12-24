#ifndef ANIMATEDPROGRESSBAR_H
#define ANIMATEDPROGRESSBAR_H

#include <QProgressBar>
#include <QPainter>
#include <QStyleOption>
#include <QString>

class AnimatedProgressBar : public QProgressBar
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    explicit AnimatedProgressBar(QWidget *parent = nullptr);

    void setColor (QColor color)
    {
        setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    }

    QColor color()
    {
       return Qt::black; // getter is not really needed for now
    }

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ANIMATEDPROGRESSBAR_H
