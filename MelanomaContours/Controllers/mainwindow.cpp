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

    Mat sourceMat = helper.QImageToCvMat(sourceImage);
    Mat destMat(sourceMat.size(),sourceMat.type());
    Mat src_gray;
    Mat detected_edges;

    cvtColor(sourceMat, src_gray, CV_BGR2GRAY);

    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, 40, 600, 5 );

    destMat = Scalar::all(0);
    sourceMat.copyTo( destMat, detected_edges);

//    std::vector<std::vector<Point> > contours;
//    std::vector<Vec4i> hierarchy;
//    findContours( detected_edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

    QImage destImage = helper.QImageFromMat(destMat);
    ui->imageView->setOverlayImage(destImage);
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
