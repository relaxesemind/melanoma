#include "helper.h"
#include <QStack>
#include <QPoint>
#include <iostream>
#include <algorithm>
#include "Models/single_area.h"
#include "Models/appstorage.h"
#include "Managers/sectorsprocess.h"

Helper::Helper(QObject *parent) : QObject(parent)
{
    threadPool = new QThreadPool();
}

Helper::~Helper()
{
    delete threadPool;
}

cv::Mat Helper::QImageToCvMat(const QImage &inImage, bool inCloneImageData)
{
   switch ( inImage.format() )
   {
      // 8-bit, 4 channel
      case QImage::Format_ARGB32:
      case QImage::Format_ARGB32_Premultiplied:
      {
         cv::Mat  mat( inImage.height(), inImage.width(),
                       CV_8UC4,
                       const_cast<uchar*>(inImage.bits()),
                       static_cast<size_t>(inImage.bytesPerLine())
                       );

         return (inCloneImageData ? mat.clone() : mat);
      }

      // 8-bit, 3 channel
      case QImage::Format_RGB32:
      {
         if ( !inCloneImageData )
         {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
         }

         cv::Mat  mat( inImage.height(), inImage.width(),
                       CV_8UC4,
                       const_cast<uchar*>(inImage.bits()),
                       static_cast<size_t>(inImage.bytesPerLine())
                       );

         cv::Mat  matNoAlpha;

         cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

         return matNoAlpha;
      }

      // 8-bit, 3 channel
      case QImage::Format_RGB888:
      {
         if ( !inCloneImageData )
         {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
         }

         QImage   swapped = inImage.rgbSwapped();

         return cv::Mat( swapped.height(), swapped.width(),
                         CV_8UC3,
                         const_cast<uchar*>(swapped.bits()),
                         static_cast<size_t>(swapped.bytesPerLine())
                         ).clone();
      }

      // 8-bit, 1 channel
      case QImage::Format_Indexed8:
      {
         cv::Mat  mat( inImage.height(), inImage.width(),
                       CV_8UC1,
                       const_cast<uchar*>(inImage.bits()),
                       static_cast<size_t>(inImage.bytesPerLine())
                       );

         return (inCloneImageData ? mat.clone() : mat);
      }

      default:
         qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
         break;
   }

   return cv::Mat();
}

cv::Mat Helper::QPixmapToCvMat(const QPixmap &inPixmap, bool inCloneImageData)
{
    return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
}

QImage Helper::QImageFromMat(const cv::Mat &inMat)
{
//    QImage image(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_RGB888);
//    return image.rgbSwapped();
    switch (inMat.type())
        {
            // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image(inMat.data,
                inMat.cols, inMat.rows,
                static_cast<int>(inMat.step),
                QImage::Format_ARGB32);

            return image;
        }

        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image(inMat.data,
                inMat.cols, inMat.rows,
                static_cast<int>(inMat.step),
                QImage::Format_RGB888);

            return image.rgbSwapped();
        }

        // 8-bit, 1 channel
        case CV_8UC1:
        {
            QImage image(inMat.data, inMat.cols, inMat.rows,
                         static_cast<int>(inMat.step),
                         QImage::Format_Grayscale8);
            return image;
        }

        default:
            qWarning() << "cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
        }

        return QImage();
}

QImage Helper::gaussianBlur(const cv::Mat &mat)
{
     cv::GaussianBlur(mat, mat, cv::Size(9,9), 0, 0);
     return this->QImageFromMat(mat);
}

void Helper::findLines(const QImage &binarImage)
{
    const qint32 _w = binarImage.width();
    const qint32 _h = binarImage.height();
    qint64 L = 1;
    std::vector<std::vector<qint64>> Labels(_h, std::vector<qint64>(_w, 0));

    for (int y = 0; y < _h; ++y)
        for (int x = 0; x < _w; ++x)
        {
            if (Labels[y][x] == 0 && binarImage.pixel(x,y) == qRgb(255,255,255))
            {
                fill(binarImage, Labels, x, y, L++);
            }
        }

//    for (int y = 0; y < _h; ++y)
//    {
//        for (int x = 0; x < _w; ++x)
//        {
//            std::cout << Labels[y][x] << " ";
//        }
//        std::cout << std::endl;
//    }

    const quint64 size = --L; // size == num of objects
    QVector<S_area> V(size);

    for(quint64 i = 0;i < size; ++i)
     {
       V[i] = S_area(i);
     }

    if ((size > 0))
    {
    for(int y = 1; y < _h; ++y)//general case
      for(int x = 1; x < _w; ++x)//general case
      {
          auto id = Labels[y][x];
          if ((id > 0)&&(id < size + 1))
          {
            QPoint t(x,y);
            V[id - 1].add_main(t);
          }
      }
    }
//     qDebug() << V.size();
    int index = 0;

    for (int i = 0; i < V.size(); ++i)
    {
        if (V[i].Points.size() > 4)
        {
            V[i].id = index++;
            AppStorage::shared().lines.append(V[i]);
        }
    }
}
void Helper::preparePointsForGraph(int type, int factor)
{
    auto& storage = AppStorage::shared();

    if (storage.lines.isEmpty() or factor == 0)
    {
        return;
    }

    SectorsProcess *process = new SectorsProcess();
    QObject::connect(process, &SectorsProcess::sectorsEmitted, this, [&storage, this, type](){
        auto& sectors = storage.sectors;
        QVector<QPointF> points;
        std::for_each(sectors.begin(), sectors.end(),[&storage, &points, type](Sector& sector){
            switch (type) {
            case 0:
            {
               qreal len = sector.averageLength();
               qreal globalen = storage.averageLenght;
               points.append(QPointF(sector.id, len - globalen));
            } break;

            default:
                break;
            }
        });

        emit this->pointsEmitted(points);
    });

    threadPool->start(process);
}

void Helper::fill(const QImage &img, std::vector<std::vector<qint64> > &labels, qint32 _x, qint32 _y, qint64 L)
{
    QPoint t;
    QStack<QPoint> depth;
    depth.push({_x, _y});
    const qint32 w = img.width();
    const qint32 h = img.height();

    auto check = [&](QPoint point)
    {
        qint32 x = point.x();
        qint32 y = point.y();
        if ((x + 1 < w)&&(x > 0)&&(y > 0)&&(y + 1 < h)
                &&(img.pixel(x,y) == qRgb(255,255,255))&&(labels[y][x] == 0))
        {
            depth.push(point);
        }
    };

    while (!depth.empty())
    {
        t = depth.pop();
        qint32 x = t.rx();
        qint32 y = t.ry();
        labels[y][x] = L; // filling.

        check({x + 1, y});
        check({x, y + 1});
        check({x - 1, y});
        check({x, y - 1});
        //8d - connect
        check({x + 1, y + 1});
        check({x - 1, y - 1});
        check({x + 1, y - 1});
        check({x - 1, y + 1});

        if (depth.size() > 2)//knot point
        {
            labels[y][x] = -1;
            return;
        }
    }
}









