#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Models/appstorage.h"
#include <QFileDialog>
#include <QDebug>

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

    IplImage *image = cvLoadImage(path.toStdString().c_str(),1);
    IplImage *gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    IplImage *dst = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );


    // окно для отображения картинки
   // cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
   // cvNamedWindow("gray",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("cvCanny",CV_WINDOW_AUTOSIZE);

    // преобразуем в градации серого
    cvCvtColor(image, gray, CV_RGB2GRAY);

    // получаем границы
//    cvCanny( const CvArr* image, CvArr* edges, double threshold1,
//                         double threshold2, int  aperture_size CV_DEFAULT(3) );
    cvCanny(gray, dst, 10, 700, 5);

    // показываем картинки
  //  cvShowImage("original",image);
   // cvShowImage("gray",gray);
    cvShowImage("cvCanny", dst );

    // ждём нажатия клавиши
    cvWaitKey(0);

    // освобождаем ресурсы
    cvReleaseImage(&image);
    cvReleaseImage(&gray);
    cvReleaseImage(&dst);
    // удаляем окна
    cvDestroyAllWindows();
}
