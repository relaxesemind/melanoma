#ifndef SECTOR_H
#define SECTOR_H

#include <QVector>
#include <QPainterPath>
#include <utility>

struct Sector
{
    explicit Sector(int id);
    Sector() = default;

    int id;
    QVector<int> linesIds;
    bool isContainsPoint(QPoint point);
    qreal averageLength();
    qreal averageWidth();
    QColor averageColor();

    std::pair<int, int> getRadSec(); // radius index, alpha index
};

#endif // SECTOR_H
