#include "paramscalculator.h"
#include <algorithm>

ParamsCalculator::ParamsCalculator()
{

}

void ParamsCalculator::calculateAllParams()
{
    auto& storage = AppStorage::shared();
    auto& lines = storage.lines;
    auto& sourceImage = storage.sourceImage;

    if (lines.isEmpty())
    {
        return;
    }

    qreal averageLenght = 0;;
    qreal averageThick = 0;
    QColor averageColor;
    qreal averageAngle = 0;
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f;
    float scale1 = 0xFF * lines.count();

    std::for_each(lines.begin(), lines.end(),[&](S_area& line) {
        if (line.Points.size() < 2)
        {
            return ;
        }
        float r = 0.0f, g = 0.0f, b = 0.0f;
        float scale = 0xFF * line.Points.count();
        std::for_each(line.Points.begin(), line.Points.end(),[&sourceImage, &r, &g, &b](QPoint point){
            QRgb pixel = sourceImage.pixel(point.x(), point.y());
            r += qRed(pixel);
            g += qGreen(pixel);
            b += qBlue(pixel);
        });

       line.color = QColor::fromRgbF(r / scale, g / scale, b / scale);
       averageAngle += line.getAngle();
       averageLenght += line.getLenght();
       line.thickness = line.getLenght() * 0.38;
       QRgb asd = line.color.rgb();
       r1 += qRed(asd);
       g1 += qGreen(asd);
       b1 += qBlue(asd);
    });

    averageAngle /= lines.size();
    averageLenght /= lines.size();
    averageColor = QColor::fromRgbF(r1 / scale1, g1 / scale1, b1 / scale1);
    averageThick = averageLenght * 0.38;

    storage.averageAngle = averageAngle;
    storage.averageLenght = averageLenght;
    storage.averageColor = averageColor;
    storage.averageThick = averageThick;
}
