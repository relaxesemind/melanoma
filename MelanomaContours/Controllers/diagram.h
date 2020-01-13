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

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

private:
    Ui::Diagram *ui;

    void updateAverageLabels();
    void updateGraph(int index);
};

#endif // DIAGRAM_H
