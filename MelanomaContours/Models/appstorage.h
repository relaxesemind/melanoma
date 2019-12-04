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


enum ColorModel
{
    RGB, HSV, LAB
};


class AppStorage : public SingletonClass<AppStorage>
{
    friend class SingletonClass<AppStorage>;
    AppStorage();

public:
    QString imagePath;
    QImage sourceImage;
    QVector<S_area> lines;
    QPolygon spotArea;
    qreal averageLenght;
    qreal averageThick;
    QColor averageColor;
    qreal averageAngle;
};

#endif // APPSTORAGE_H
