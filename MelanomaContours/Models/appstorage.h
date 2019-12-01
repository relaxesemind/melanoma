#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"

#include <QVector>
#include <QString>
#include <QVector3D>
#include <QMap>
#include <utility>
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
    QVector<S_area> lines;
};

#endif // APPSTORAGE_H
