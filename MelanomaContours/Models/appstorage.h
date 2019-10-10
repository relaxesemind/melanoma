#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"

#include <QVector>
#include <QString>
#include <QVector3D>
#include <QMap>
#include <utility>


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
};

#endif // APPSTORAGE_H
