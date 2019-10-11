#ifndef HELPER_H
#define HELPER_H


#include "Common/opencv.h"
#include <QImage>
#include <QDebug>
#include <QPixmap>


class Helper
{
public:
    Helper() = default;

    cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
    cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true );
    QImage  QImageFromMat(const cv::Mat& mat);

};

#endif // HELPER_H
