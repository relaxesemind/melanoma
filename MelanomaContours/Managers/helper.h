#ifndef HELPER_H
#define HELPER_H


#include "Common/opencv.h"
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <vector>
#include <QVector>


class Helper
{
public:
    Helper() = default;

    cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
    cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true );
    QImage  QImageFromMat(const cv::Mat& mat);
    QImage  gaussianBlur(const cv::Mat& mat);
    void findLines(const QImage& binarImage);
    QVector<QPointF> preparePointsForGraph(int type, int factor = 3);

private:
    void fill(const QImage& img, std::vector<std::vector<qint64>>& labels , qint32 _x, qint32 _y, qint64 L);

};

#endif // HELPER_H
