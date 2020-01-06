#include "appstorage.h"
#include "Managers/managerslocator.h"
#include "Common/consts.h"

AppStorage::AppStorage()
{
    numberOfRadius = Global::defaultNumberOfRadius;
    numberOfSectors = Global::defaultNumberOfSectors;
    t = Global::breadleyRot_t_factor;
    S = Global::bredleyRotSfactor;
}
