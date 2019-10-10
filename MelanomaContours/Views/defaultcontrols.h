#ifndef DEFAULTCONTROLS_H
#define DEFAULTCONTROLS_H


#include "Abstract/singletoneclass.h"
#include "Common/magic.h"


#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>
#include <QDate>
#include <type_traits>


class DefaultControls : public SingletonClass<DefaultControls>
{
    friend class SingletonClass<DefaultControls>;
    DefaultControls() = default;

public:
    template<class Action> void showConfirmDialog(const QString& title, Action okAction);
    template<class Action> void showStringInputDialog(const QString& title, Action okAction);
                           void showMessage(const QString& title, const QString& description);
};

template<class Action>
void DefaultControls::showConfirmDialog(const QString &title, Action okAction)
{
//    static_assert(std::is_function<Action>::value,"okAction must be a callable");

    QMessageBox _box;
    QPushButton *_okBut = _box.addButton(QMessageBox::Ok);
    QPushButton *_cancelBut = _box.addButton(QMessageBox::Cancel);
    _box.setText(title);
    _box.exec();

    if (_box.clickedButton() == _okBut)
    {
        okAction();
    }

    if (_box.clickedButton() == _cancelBut)
    {
        qDebug() << "cancel";
    }
}

template<class Action>
void DefaultControls::showStringInputDialog(const QString& title, Action okAction)
{
//    static_assert(std::is_i <Action>::value,"okAction must be a callable");

    bool ok;

    QString text = QInputDialog::getText(0, "Введите ",title, QLineEdit::Normal, "", &ok);
    if (ok and !text.isEmpty())
    {
        okAction(text);
    }
}


#endif // DEFAULTCONTROLS_H
