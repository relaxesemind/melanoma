#ifndef MANAGERSLOCATOR_H
#define MANAGERSLOCATOR_H

#include "Abstract/singletoneclass.h"
#include "colorgenerator.h"
#include "mathmanager.h"


class ManagersLocator : public SingletonClass<ManagersLocator>
{
    friend class SingletonClass<ManagersLocator>;
    ManagersLocator() = default;
public:

    ColorGenerator<> colorGenerator;
    MathManager mathManager;
};

#endif // MANAGERSLOCATOR_H
