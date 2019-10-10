#ifndef COLORGENERATOR_H
#define COLORGENERATOR_H

#include <random>
#include <chrono>
#include <QColor>
#include <QRgb>


template <
          class engine = std::default_random_engine,
          class distribution = std::uniform_int_distribution<uint>
          >
class ColorGenerator
{
public:
    ColorGenerator():dist(MIN,MAX)
    {
        auto s = std::chrono::system_clock::now().time_since_epoch().count();
        re.seed(s);
    }

    uint operator()()
    {
        return dist(re);
    }

    operator QRgb()
    {
        return static_cast<QRgb>(this->operator()());
    }

    operator QColor()
    {
        return QColor(QRgb(*this));
    }

    QColor get()
    {
        uint vl = dist(re);
        return QColor(static_cast<QRgb>(vl));
    }

private:
    const uint MIN = 0x0;
    const uint MAX = 0xFFFFFF;
    engine re;
    distribution dist;
};

#endif // COLORGENERATOR_H
