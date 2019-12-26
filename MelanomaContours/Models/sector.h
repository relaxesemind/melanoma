#ifndef SECTOR_H
#define SECTOR_H

#include <QVector>
#include <QPainterPath>
#include <utility>

struct Sector
{
    explicit Sector(int id);

    int id;
    QVector<int> linesIds;
    bool isContainsPoint(QPoint point);

private:
    std::pair<int, int> getRadSec(); // radius index, alpha index
};

#endif // SECTOR_H
