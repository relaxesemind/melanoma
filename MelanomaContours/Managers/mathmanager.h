#ifndef MATHMANAGER_H
#define MATHMANAGER_H

#include <QRgb>
#include <QVector3D>
#include <QVector2D>
#include <QImage>
#include "Models/appstorage.h"
#include "Common/opencv.h"

enum pointPosOverPlane
{
    front,
    behind,
    into
};

class MathManager
{
public:
    MathManager() = default;
    void rgb2lab( float R, float G, float B, float & l_s, float &a_s, float &b_s );
    void rgb2hsv(QRgb q, float &h, float &s, float &v);
    cv::Mat imagePreparation(const QImage& sourceImage);
};

#endif // MATHMANAGER_H
