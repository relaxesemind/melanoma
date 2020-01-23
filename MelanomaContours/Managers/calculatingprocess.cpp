#include "calculatingprocess.h"
#include "Models/appstorage.h"
#include "Managers/managerslocator.h"

CalculatingProcess::CalculatingProcess(const QImage& image) : sourceImage(image)
{

}

void CalculatingProcess::run()
{
    emit isRunning(true);
    emit progress(0);

    if (sourceImage.isNull())
    {
        emit isDone(false, QImage());
        return;
    }

    auto& helper = ManagersLocator::shared().helper;
    auto& storage = AppStorage::shared();
    QImage binarImage = ManagersLocator::shared().mathManager.thresholdBradley(sourceImage);

    for (int y = 0; y < binarImage.height(); ++y)
        for(int x = 0; x < binarImage.width(); ++x)
        {
            int x_0 = x - storage.centerPointArea.x();
            int y_0 = y - storage.centerPointArea.y();
            qreal r = storage.areaRadius;

            if (x_0 * x_0 + y_0 * y_0 > r * r)
            {
                binarImage.setPixel(x, y, 0xFF000000);
            }
        }

    emit progress(15);

    ManagersLocator::shared().skeletFilter.doFilter(binarImage);
    emit progress(65);
    helper.findLines(binarImage);
    emit progress(75);

    auto& lines = storage.lines;
    QImage copy(binarImage.size(), QImage::Format_ARGB32);
    copy.fill(QColor("transparent"));
    for (int i = 0; i < lines.count(); ++i)
    {
        auto& line = lines[i];
        QColor color = ManagersLocator::shared().colorGenerator.get();
        for (int j = 0; j < line.Points.size(); ++j)
        {
            QPoint point = line.Points[j];
            copy.setPixel(point.x(), point.y(), color.rgb());
        }
    }

    emit progress(85);
    ManagersLocator::shared().paramsCalc.calculateAllParams();
    emit progress(100);

    emit isRunning(false);
    emit progress(0);
    emit isDone(true, copy);
//    emit isDone(true, binarImage);
}
