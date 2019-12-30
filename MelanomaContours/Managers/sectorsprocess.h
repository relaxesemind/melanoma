#ifndef SECTORSPROCESS_H
#define SECTORSPROCESS_H

#include <QObject>
#include <QRunnable>

class SectorsProcess : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit SectorsProcess(QObject *parent = nullptr);

protected:
    void run();

private:

signals:
    void sectorsEmitted();

public slots:
};

#endif // SECTORSPROCESS_H
