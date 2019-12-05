#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Models/appstorage.h"
#include "Managers/managerslocator.h"
#include "Controllers/diagram.h"
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
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "*.jpg *.jpeg *.bmp *.png"); //"C:/Users/relaxes/Documents/MEPHI/46_KAF/!!!!!!!!!!!!15COURSE!!!!!!!!1/PRONI/melanoma/Gmail/121.jpg";
    //QFileDialog::getOpenFileName(this, "Выберите изображение", "", "*.jpg *.jpeg *.bmp *.png");
    if (!fileName.isEmpty())
    {
        AppStorage::shared().imagePath = fileName;
        QPixmap pixmap;
        pixmap.load(fileName);
        QImage sourceImage = pixmap.toImage();
        AppStorage::shared().sourceImage = sourceImage;
        ui->imageView->setImage(sourceImage);
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

    auto& helper = ManagersLocator::shared().helper;

    cv::Mat src = helper.QImageToCvMat(sourceImage);

    QImage  blurredImage = helper.gaussianBlur(src);
//    ui->imageView->setImage(sharpen);

    QImage binarImage = ManagersLocator::shared().mathManager.thresholdBradley(sourceImage);
//     ui->imageView->setImage(binarImage);

    ManagersLocator::shared().skeletFilter.doFilter(binarImage);
    helper.findLines(binarImage);
    auto& lines = AppStorage::shared().lines;
    QImage copy(binarImage.size(), QImage::Format_ARGB32);
    copy.fill(QColor("transparent"));
    for (int i = 0; i < lines.count(); ++i)
    {
        auto& line = lines[i];
        QColor color = ManagersLocator::shared().colorGenerator.get();
        for (int j = 0; j < line.Points.size(); ++j)
        {
            QPoint point = line.Points[j];
            copy.setPixel(point.x(),point.y(), color.rgb());
        }
    }
    ui->imageView->setOverlayImage(copy);
    ManagersLocator::shared().paramsCalc.calculateAllParams();
    ui->pushButton_2->setEnabled(true);


//    ui->imageView->setOverlayImage(binarImage);
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
