#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"

#include <QVector>
#include <QString>
#include <QVector3D>
#include <QMap>
#include <utility>
#include <QPolygon>
#include <QImage>
#include "Models/single_area.h"
#include "Models/sector.h"


enum ColorModel
{
    RGB, HSV, LAB
};


class AppStorage : public SingletonClass<AppStorage>
{
    friend class SingletonClass<AppStorage>;
    AppStorage();

public:
    void clearStorage();

    QString imagePath;
    QImage sourceImage, nevusImage;
    QVector<S_area> lines;
    QPolygon spotArea;
    qreal averageLenght;
    qreal averageThick;
    QColor averageColor;
    qreal averageAngle;

    qreal areaRadius;
    QPoint centerPointArea;
    int numberOfRadius, numberOfSectors;

    QVector<qreal> radiuses; // r0 = 0
    QVector<qreal> angles; //

    QVector<Sector> sectors;

    float t;
    int S;
    int lenghtFilterValue;

    bool oXaxisMode;

    QMap<QString, QVector<QVector<quint64>>> integrals;
    QVector<QVector<qreal>> V;
};

#endif // APPSTORAGE_H
