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

    QImage sourceImage = AppStorage::shared().sourceImage;

    if (sourceImage.isNull())
    {
        return;
    }

    auto& helper = ManagersLocator::shared().helper;
    QImage binarImage = ManagersLocator::shared().mathManager.thresholdBradley(sourceImage);
    emit progress(15);

    ManagersLocator::shared().skeletFilter.doFilter(binarImage);
    emit progress(65);
    helper.findLines(binarImage);
    emit progress(75);

    auto& lines = AppStorage::shared().lines;
    QImage copy(binarImage.size(), QImage::Format_ARGB32);
    copy.fill(QColor("transparent"));
    for (int i = 0; i < lines.count(); ++i)
    {
        auto& line = lines[i];
        QColor color = ManagersLocator::shared().colorGenerator.get();
        for (int j = 0; j < line.Points.size(); ++j)
        {
            QPoint point = line.Points[j];
            copy.setPixel(point.x(),point.y(), color.rgb());
        }
    }

    emit progress(85);
    ManagersLocator::shared().paramsCalc.calculateAllParams();
    emit progress(100);

    emit isRunning(false);
    emit progress(0);
    emit isDone(true, copy);
}
