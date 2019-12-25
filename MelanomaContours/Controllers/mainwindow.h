#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThreadPool>
#include "Common/opencv.h"
#include "Views/waitingspinnerwidget.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadImageAction_triggered();

    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_horizontalSlider_2_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QThreadPool *threadPool;
    WaitingSpinnerWidget *spinner;
    void runMainProcess();
    void drawSectors();
};

#endif // MAINWINDOW_H
