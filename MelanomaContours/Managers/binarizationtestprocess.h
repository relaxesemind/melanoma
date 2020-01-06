#ifndef BINARIZATIONTESTPROCESS_H
#define BINARIZATIONTESTPROCESS_H

#include <QObject>
#include <QRunnable>
#include <QImage>

class BinarizationTestProcess : public QObject , public QRunnable
{
    Q_OBJECT

public:
    explicit BinarizationTestProcess(const QImage& image);

protected:
    void run();

private:
    QImage sourceImage;

signals:
    void isDone(bool status, QImage dest);
    void progress(int percent);
    void isRunning(bool);
};

#endif // BINARIZATIONTESTPROCESS_H
