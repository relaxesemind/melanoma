#include "appstorage.h"
#include "Managers/managerslocator.h"
#include "Common/consts.h"

AppStorage::AppStorage()
{
    numberOfRadius = Global::defaultNumberOfRadius;
    numberOfSectors = Global::defaultNumberOfSectors;
    t = Global::breadleyRot_t_factor;
    S = Global::bredleyRotSfactor;
    lenghtFilterValue = 3;
    K.resize(4);
    oXaxisMode = false;
}

void AppStorage::clearStorage()
{
    imagePath.clear();
    sourceImage = QImage();
    nevusImage = QImage();
    lines.clear();
    K.clear();
    K.resize(4);
    spotArea = QPolygon();
    averageAngle = 0;
    averageColor = QColor();
    averageLenght = 0;
    averageThick = 0;
    areaRadius = 0;
    centerPointArea = QPoint();
    radiuses.clear();
    angles.clear();
    sectors.clear();
}
