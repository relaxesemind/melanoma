#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Models/appstorage.h"
#include "Managers/managerslocator.h"
#include <QFileDialog>
#include <QDebug>
#include <vector>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImageAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "*.jpg *.jpeg *.bmp *.png");//"C:/Users/relaxes/Documents/MEPHI/46_KAF/!!!!!!!!!!!!15COURSE!!!!!!!!1/PRONI/melanoma/Gmail/121.jpg";
    //QFileDialog::getOpenFileName(this, "Выберите изображение", "", "*.jpg *.jpeg *.bmp *.png");
    if (!fileName.isEmpty())
    {
        AppStorage::shared().imagePath = fileName;
        QPixmap pixmap;
        pixmap.load(fileName);
        ui->imageView->setImage(pixmap.toImage());
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString path = AppStorage::shared().imagePath;
    QImage sourceImage = ui->imageView->getImage();

    if (sourceImage.isNull())
    {
        return;
    }

//    Mat res = ManagersLocator::shared().mathManager.imagePreparation(sourceImage);
//    QImage destImage = ManagersLocator::shared().helper.QImageFromMat(res);

//    ui->imageView->setImage(destImage);
  //  QImage binarDestImage = ManagersLocator::shared().mathManager.dummyThreshold(destImage);
//    ui->imageView->setImage(destImage);

//    ManagersLocator::shared().skeletFilter.doFilter(destImage);
   // ui->imageView->setOverlayImage(binarDestImage);

    QImage bin = ManagersLocator::shared().mathManager.thresholdBradley(sourceImage);
//    ui->imageView->setImage(bin);
    ManagersLocator::shared().skeletFilter.doFilter(bin);
    ui->imageView->setOverlayImage(bin);
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
