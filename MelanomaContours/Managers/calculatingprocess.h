#ifndef CALCULATINGPROCESS_H
#define CALCULATINGPROCESS_H

#include <QObject>
#include <QRunnable>
#include <QImage>


class CalculatingProcess : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit CalculatingProcess(const QImage& image);

protected:
    void run();

private:
    QImage sourceImage;

signals:
    void isDone(bool status, QImage dest);
    void progress(int percent);
    void isRunning(bool);
};

#endif // CALCULATINGPROCESS_H
