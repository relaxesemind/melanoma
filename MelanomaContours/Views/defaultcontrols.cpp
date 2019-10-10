#include "defaultcontrols.h"

void DefaultControls::showMessage(const QString &title, const QString &description)
{
    QMessageBox::information(nullptr, title, description);
}
