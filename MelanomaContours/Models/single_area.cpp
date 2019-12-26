#include "single_area.h"
#include <cmath>
#include <QVector2D>

qreal S_area::getAngle()
{
    if (angle < 0 && Points.count() > 1)
    {
        QPoint first = Points.first();
        QPoint last = Points.last();
        qreal x = last.x() - first.x();
        qreal y = last.y() - first.y();
        if (x != 0)
        {
            angle = std::atan(y / x) * (180. / M_PI);
        }
    }

    return angle;
}

qreal S_area::getLenght()
{
    if (lenght < 0 && Points.count() > 1)
    {
        QPoint first = Points.first();
        QPoint last = Points.last();

        lenght = std::sqrt(std::pow(last.x() - first.x(), 2) + std::pow(last.y() - first.y(), 2));
    }

    return lenght;
}

QPoint S_area::getCenter()
{
    if (Points.isEmpty())
    {
        return QPoint();
    }

    if (Points.count() < 3)
    {
        return Points.first();
    }

    return Points.at(Points.count() / 2);
}
