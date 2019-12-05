#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QWidget>

namespace Ui {
class Diagram;
}

class Diagram : public QWidget
{
    Q_OBJECT

public:
    explicit Diagram(QWidget *parent = 0);
    ~Diagram();

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Diagram *ui;

    void updateAverageLabels();
};

#endif // DIAGRAM_H
