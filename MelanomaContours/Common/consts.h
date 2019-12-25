#ifndef CONSTS_H
#define CONSTS_H

#include <QObject>
#include <QString>

namespace Global
{
    const qreal zoomMul = 1.05;
    const int bredleyRotSfactor = 14;
    const float breadleyRot_f_factor = 0.06f;
    const QString progressBarStyle(
            "QProgressBar:horizontal {"
            "border-radius: 3px;"
            "background: white;"
            "padding: 1px;"
            "}"
            "QProgressBar::chunk:horizontal {"
            "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 #FF893D, stop: 1 white);"
            "}");
}

#endif // CONSTS_H
