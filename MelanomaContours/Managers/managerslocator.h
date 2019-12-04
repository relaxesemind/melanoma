#ifndef MANAGERSLOCATOR_H
#define MANAGERSLOCATOR_H

#include "Abstract/singletoneclass.h"
#include "colorgenerator.h"
#include "mathmanager.h"
#include "helper.h"
#include "skiletizationfilter.h"
#include "paramscalculator.h"


class ManagersLocator : public SingletonClass<ManagersLocator>
{
    friend class SingletonClass<ManagersLocator>;
    ManagersLocator() = default;
public:

    ColorGenerator<> colorGenerator;
    MathManager mathManager;
    Helper helper;
    SkiletizationFilter skeletFilter;
    ParamsCalculator paramsCalc;
};

#endif // MANAGERSLOCATOR_H
