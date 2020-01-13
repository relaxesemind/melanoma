#ifndef HELPER_H
#define HELPER_H


#include "Common/opencv.h"
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <vector>
#include <QVector>
#include <QThreadPool>
#include <QObject>


class Helper : public QObject
{
    Q_OBJECT
public:
    Helper(QObject *parent = nullptr);
    ~Helper();

    cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
    cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true );
    QImage  QImageFromMat(const cv::Mat& mat);
    QImage  gaussianBlur(const cv::Mat& mat);
    void findLines(const QImage& binarImage);
    void preparePointsForGraph(int type, bool mode, int factor = 3);

signals:
    void pointsEmitted(QVector<QVector<QPointF>>);

private:
    void fill(const QImage& img, std::vector<std::vector<qint64>>& labels , qint32 _x, qint32 _y, qint64 L);
    QThreadPool *threadPool;
    qreal distance(const QColor& a, const QColor& b);

};

#endif // HELPER_H
