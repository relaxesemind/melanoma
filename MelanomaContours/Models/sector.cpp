#include "sector.h"
#include "Models/appstorage.h"

#include <cmath>

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

    if (R.count() == 1 || i == 0)
    {
        cond1 = r <= R.first();
    }
    else if (i > R.count() - 1)
    {
        cond1 = false;
    }
    else
    {
        cond1 = R[i] <= r <= R[i + 1];
    }

    bool cond2 = false;

    if (j >= A.count() - 1)
    {
        cond2 = A[j] <= fi <= A[0] - A[j];
    }
    else
    {
        cond2 = A[j] <= fi <= A[j + 1] - A[j];
    }

    return cond1 && cond2;
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
