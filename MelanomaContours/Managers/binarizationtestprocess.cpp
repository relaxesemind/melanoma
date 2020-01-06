#include "binarizationtestprocess.h"
#include "Models/appstorage.h"
#include "Managers/managerslocator.h"

BinarizationTestProcess::BinarizationTestProcess(const QImage &image) : sourceImage(image)
{

}

void BinarizationTestProcess::run()
{
    emit isRunning(true);

    if (sourceImage.isNull())
    {
        emit isDone(false, QImage());
        return;
    }

    auto& helper = ManagersLocator::shared().helper;
    auto& storage = AppStorage::shared();
    QImage binarImage = ManagersLocator::shared().mathManager.thresholdBradley(sourceImage);

    emit isRunning(false);
    emit isDone(true, binarImage);
}
