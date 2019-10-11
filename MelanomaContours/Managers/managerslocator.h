#ifndef MANAGERSLOCATOR_H
#define MANAGERSLOCATOR_H

#include "Abstract/singletoneclass.h"
#include "colorgenerator.h"
#include "mathmanager.h"
#include "helper.h"


class ManagersLocator : public SingletonClass<ManagersLocator>
{
    friend class SingletonClass<ManagersLocator>;
    ManagersLocator() = default;
public:

    ColorGenerator<> colorGenerator;
    MathManager mathManager;
    Helper helper;
};

#endif // MANAGERSLOCATOR_H
