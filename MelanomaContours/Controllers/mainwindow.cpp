#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Models/appstorage.h"
#include "Managers/managerslocator.h"
#include "Controllers/diagram.h"
#include "Managers/calculatingprocess.h"
#include "Common/consts.h"
#include <QFileDialog>
#include <QDebug>
#include <vector>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    threadPool(new QThreadPool(this))
{
    ui->setupUi(this);
    spinner = nullptr;
    ui->progressBar->setStyleSheet(Global::progressBarStyle);
}

MainWindow::~MainWindow()
{
    delete threadPool;
    delete ui;
}

void MainWindow::on_loadImageAction_triggered()
{
    QString fileName = "C:/Users/relaxes/Documents/MEPHI/46_KAF/!!!!!!!!!!!!15COURSE!!!!!!!!1/PRONI/melanoma/Gmail/121.jpg";
    //QFileDialog::getOpenFileName(this, "Выберите изображение", "", "*.jpg *.jpeg *.bmp *.png");
    if (!fileName.isEmpty())
    {
        auto& storage = AppStorage::shared();
        storage.imagePath = fileName;
        QPixmap pixmap;
        pixmap.load(fileName);
        QImage sourceImage = pixmap.toImage();
        storage.sourceImage = sourceImage;
        storage.nevusImage = ManagersLocator::shared().mathManager.pigmentArea(sourceImage).first;

        ui->imageView->setImage(sourceImage);
    }
}

void MainWindow::on_pushButton_clicked()
{
    auto pair = ManagersLocator::shared().mathManager.centerOfPigmentArea(AppStorage::shared().nevusImage);
    ui->imageView->drawSectors(pair.first, pair.second);
    runMainProcess();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)//opacity
{
    if (ui->imageView->getOverlayImage().isNull())
    {
        return;
    }

    qreal op = value / 100.;
    ui->imageView->setOpacity(op);
}

void MainWindow::on_pushButton_2_clicked()
{
    (new Diagram())->show();
}

void MainWindow::runMainProcess()
{
    QImage image = AppStorage::shared().sourceImage;
    CalculatingProcess *process = new CalculatingProcess(image);

    connect(process, &CalculatingProcess::isRunning, this,[this](bool isRunning){
        if (isRunning)
        {
            spinner = new WaitingSpinnerWidget(ui->imageView, Qt::ApplicationModal, true);
            spinner->setRoundness(70.0);
            spinner->setMinimumTrailOpacity(15.0);
            spinner->setTrailFadePercentage(70.0);
            spinner->setNumberOfLines(12);
            spinner->setLineLength(12);
            spinner->setLineWidth(4);
            spinner->setInnerRadius(10);
            spinner->setRevolutionsPerSecond(1);
            spinner->start();
            ui->pushButton->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
        }
        else if (spinner != nullptr)
        {
            spinner->stop();
            delete spinner;
            spinner = nullptr;
        }
    });

    connect(process, &CalculatingProcess::isDone, this, [this](bool isDone, QImage image){
        if (!isDone)
        {
            return ;
        }
        ui->imageView->setOverlayImage(image);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton->setEnabled(true);
    });

    connect(process, &CalculatingProcess::progress, this, [this](int percent){
        ui->progressBar->setValue(percent);
    });

    threadPool->start(process);
}

void MainWindow::drawSectors()
{

}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    AppStorage::shared().numberOfRadius = value;
    auto pair = ManagersLocator::shared().mathManager.centerOfPigmentArea(AppStorage::shared().nevusImage);
    ui->imageView->drawSectors(pair.first, pair.second, value, AppStorage::shared().numberOfSectors);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    AppStorage::shared().numberOfSectors = value;
    auto pair = ManagersLocator::shared().mathManager.centerOfPigmentArea(AppStorage::shared().nevusImage);
    ui->imageView->drawSectors(pair.first, pair.second, AppStorage::shared().numberOfRadius, value);
}
