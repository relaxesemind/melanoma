#include "sector.h"
#include "Models/appstorage.h"
#include <limits>
#include <cmath>
#include <QDebug>

Sector::Sector(int id) : id(id)
{ 
}

bool Sector::isContainsPoint(QPoint point)
{
    QPoint const center = AppStorage::shared().centerPointArea;
    QPoint localPoint = point - center;
    qreal x = localPoint.x();
    qreal y = localPoint.y();

    qreal r = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    qreal fi = std::atan2(y, x);
    if (fi < 0)
    {
        fi += 2 * M_PI;
    }

    auto idx = getRadSec();
    int i = idx.first;
    int j = idx.second;
    auto& R = AppStorage::shared().radiuses;
    auto& A = AppStorage::shared().angles;

    bool cond1 = false;
    qreal R2 = std::numeric_limits<qreal>::max();
    qreal R1 = 0;

    if (R.count() == 0)
    {
        return false;
    }
    else if (R.count() == 1 || i == 0)
    {
        R2 = R.first();
    }
    else if (i > R.count() - 1)
    {
        return false;
    }
    else
    {
        R1 = R[i - 1];
        R2 = R[i];
    }

    cond1 = (R1 <= r) && (r <= R2);

    bool cond2 = false;
    qreal A1 = 0;
    qreal A2 = M_PI;

    if (A.count() == 0)
    {
        return false;
    }
    else if (j > A.count() - 1)
    {
        return false;
    }
    else if (j == 0)
    {
        A2 = A[j];
    }
    else
    {
        A1 = A[j - 1];
        A2 = A[j];
    }

    cond2 = (A1 <= fi) && (fi <= A2);

    return cond1 && cond2;
}

qreal Sector::averageLength()
{
    if (linesIds.count() == 0)
    {
        return AppStorage::shared().averageLenght;
    }

   auto& lines = AppStorage::shared().lines;
   qreal result = 0;

   for (int i = 0; i < linesIds.count(); ++i)
   {
       if (linesIds[i] >= lines.count() - 1)
       {
           continue;
       }

       result += lines[linesIds[i]].getLenght();
   }

   result /= linesIds.count();

   return result;
}

qreal Sector::averageWidth()
{
    if (linesIds.count() == 0)
    {
        return AppStorage::shared().averageThick;
    }
    auto& lines = AppStorage::shared().lines;
    qreal result = 0;

    for (int i = 0; i < linesIds.count(); ++i)
    {
        if (linesIds[i] >= lines.count() - 1)
        {
            continue;
        }

        result += lines[linesIds[i]].thickness;
    }

    result /= linesIds.count();

    return result;
}

QColor Sector::averageColor()
{
    if (linesIds.count() == 0)
    {
        return AppStorage::shared().averageColor;
    }
    auto& lines = AppStorage::shared().lines;
    float r = 0, g = 0, b = 0;
    float scale = 0xFF * lines.count();

    for (int i = 0; i < linesIds.count(); ++i)
    {
        if (linesIds[i] >= lines.count() - 1)
        {
            continue;
        }

        QRgb rgb = lines[linesIds[i]].color.rgb();

        r += qRed(rgb);
        g += qGreen(rgb);
        b += qBlue(rgb);
    }

    r /= scale;
    g /= scale;
    b /= scale;

    return QColor::fromRgbF(r, g ,b);
}

std::pair<int, int> Sector::getRadSec()
{
    const int M = AppStorage::shared().numberOfSectors;
//    const int N = AppStorage::shared().numberOfRadius;

    int i = 0;

    while (id >= M * (i + 1))
    {
        ++i;
    };

    int j = id % M;

    return std::make_pair(i, j);
}
