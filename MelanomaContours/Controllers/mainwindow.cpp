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

    //    cv::Matx33d kernel = { -1, -1, -1,
    //                           -1,  9, -1,
    //                           -1, -1, -1 };

    //    cvtColor( src, output, CV_BGR2GRAY );

    //    cv::filter2D(src, dest, -1, kernel);

    //    cv::Mat skelet = ManagersLocator::shared().helper.QImageToCvMat(bin);

    //    std::vector<std::vector<cv::Point>> contours;

    //    cv::Mat contoursOutput = skelet.clone();

    //    cv::cvtColor(contoursOutput, contoursOutput, CV_BGR2GRAY);

    //    cv::findContours(contoursOutput, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

    //    cv::Mat contourImage(skelet.size(), CV_8UC3, cv::Scalar(0,0,0));
    //        cv::Scalar colors[3];
    //        colors[0] = cv::Scalar(255, 0, 0);
    //        colors[1] = cv::Scalar(0, 255, 0);
    //        colors[2] = cv::Scalar(0, 0, 255);
    //        for (size_t idx = 0; idx < contours.size(); idx++) {
    //            cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
    //        }

    //    cv::imshow("Input Image", skelet);
    ////    cvMoveWindow("Input Image", 0, 0);
    //    cv::imshow("Contours", contourImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImageAction_triggered()
{
    QString fileName = "C:/Users/relaxes/Documents/MEPHI/46_KAF/!!!!!!!!!!!!15COURSE!!!!!!!!1/PRONI/melanoma/Gmail/121.jpg";
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

    auto& helper = ManagersLocator::shared().helper;

    cv::Mat src = helper.QImageToCvMat(sourceImage);

    QImage  blurredImage = helper.gaussianBlur(src);
//    ui->imageView->setImage(sharpen);

    QImage binarImage = ManagersLocator::shared().mathManager.thresholdBradley(sourceImage);

    ManagersLocator::shared().skeletFilter.doFilter(binarImage);
//     ui->imageView->setImage(binarImage);

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
