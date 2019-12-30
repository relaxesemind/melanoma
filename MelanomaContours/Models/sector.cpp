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
    const int x = point.x();
    const int y = point.y();

    qreal r = std::pow(std::pow(x, 2) + std::pow(y, 2), 0.5);
    qreal fi = std::atan(y / x);
    auto idx = getRadSec();
    int i = idx.first;
    int j = idx.second;
    auto& R = AppStorage::shared().radiuses;
    auto& A = AppStorage::shared().angles;

    bool cond1 = false;
    qreal R2 = std::numeric_limits<qreal>::max();
    qreal R1 = 0;

//    qDebug() << "i = " << i << " R.Count = " << R.count() << "j = " << j << " A.Count = " << A.count();

    if (R.count() == 1)
    {
        R2 = R.first();
    }
    else if (i + 1 >= R.count() - 1)
    {
        return false;
    }
    else
    {
        R1 = R[i];
        R2 = R[i + 1];
    }

    cond1 = (R1 <= r) && (r <= R2);

//    bool cond2 = false;
//    qreal A1 = 0;
//    qreal A2 = M_PI;

//    if (j + 1 > A.count() - 1)
//    {
//        cond2 = (A[j] <= fi) && (fi <= A[0] - A[j]);
//    }
//    else
//    {
//        cond2 = (A[j] <= fi) && (fi <= A[j + 1] - A[j]);
//    }



    return cond1 ;//&& cond2;
}

std::pair<int, int> Sector::getRadSec()
{
    const int M = AppStorage::shared().numberOfSectors;
//    const int N = AppStorage::shared().numberOfRadius;

    int i = 0;

    while (id > M * (++i)) {};
    int j = 1 + id % M;

    return std::make_pair(i, j);
}
