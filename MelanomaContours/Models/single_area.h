#ifndef SINGLE_AREA_H
#define SINGLE_AREA_H

#include <QVector>
#include <QPoint>
#include <QColor>


struct S_area
{
    S_area() = default;

    explicit S_area(quint64 number): id(number), Points(0),
        thickness(0), color(Qt::black), angle(-1), lenght(-1) {}

    void add_main(QPoint p)
    {
        Points.push_back(p);
    }


public:
    quint64 id;
    QVector<QPoint> Points;
    qreal thickness;
    QColor color;

    qreal getAngle();
    qreal getLenght();
    QPoint getCenter();


private:
    qreal angle;
    qreal lenght;
};

#endif // SINGLE_AREA_H


