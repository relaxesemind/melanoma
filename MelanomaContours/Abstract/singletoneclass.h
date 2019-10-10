#ifndef SINGLETONECLASS_H
#define SINGLETONECLASS_H

#include <QObject>

template <class T>
class SingletonClass
{
public:
    static T& shared()
    {
         static T instance;
         return instance;
    }

protected:
        SingletonClass() {}
        ~SingletonClass() {}
public:
        SingletonClass(SingletonClass const &) = delete;
        SingletonClass& operator=(SingletonClass const &) = delete;
};
#endif // SINGLETONECLASS_H
