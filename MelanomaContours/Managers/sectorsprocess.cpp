#include "sectorsprocess.h"
#include "Models/appstorage.h"
#include "Models/sector.h"

SectorsProcess::SectorsProcess(QObject *parent) : QObject(parent)
{

}

void SectorsProcess::run()
{
    auto& storage = AppStorage::shared();
    auto& sectors = storage.sectors;
    sectors.clear();
    const int M = storage.numberOfSectors;
    const int N = storage.numberOfRadius;
    const int sectorsCount = M * N;

    for (int i = 0; i < sectorsCount; ++i)
    {
        sectors.append(Sector(i));
    }


    std::for_each(sectors.begin(), sectors.end(),[&](Sector& sector){
        std::for_each(storage.lines.begin(), storage.lines.end(),[&sector](const S_area& line){
            QPoint center = line.getCenter();
            if (sector.isContainsPoint(center))
            {
                sector.linesIds.append(line.id);
            }
        });
    });

    emit sectorsEmitted();
}
