#include "imageview.h"
#include <QDebug>
#include "Common/consts.h"
#include "Models/appstorage.h"
#include <QtMath>
#include "Common/magic.h"

ImageView::ImageView(QWidget *widget) : QGraphicsView(widget)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setAlignment(Qt::AlignCenter);
    currentScale = 1.0;
    opacity = 1.0;
    item = nullptr;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet(QString(
                          "background-color: #89AFD1;"
                          ));
}

QImage ImageView::getImage() const
{
    return image;
}

void ImageView::setImage(const QImage &value)
{
    image = value;
    QPixmap pixmap = QPixmap::fromImage(value);

    if (item)
    {
        scene->removeItem(item);
        item = nullptr;
    }

    item = new QGraphicsPixmapItem(pixmap);
    scene->addItem(item);

    centerOn(item);
    scene->setSceneRect(0,0,pixmap.width(),pixmap.height());
    fitInView(item,Qt::KeepAspectRatio);

    this->update();
}

void ImageView::setOpacity(qreal value)
{
    opacity = value;
    overlayItem->setOpacity(opacity);
}

void ImageView::clearView()
{
    scene->clear();
    item = nullptr;
}

QImage ImageView::getOverlayImage() const
{
    return overlayImage;
}

void ImageView::setOverlayImage(const QImage &value)
{
    overlayImage = value;
    QPixmap pix = QPixmap::fromImage(value);

    if (overlayItem)
    {
        scene->removeItem(overlayItem);
        overlayItem = nullptr;
    }

    overlayItem = new QGraphicsPixmapItem(pix);
    scene->addItem(overlayItem);

    centerOn(overlayItem);
    scene->setSceneRect(0,0,pix.width(),pix.height());
    fitInView(item,Qt::KeepAspectRatio);

    this->update();
}

QPointF ImageView::transformCoordinates(QPointF pos) const
{
    return mapToScene(pos.x(),pos.y());
}


void ImageView::wheelEvent(QWheelEvent *event)
{
    if (scene->items().isEmpty())
     {
         return;
     }

     centerOn(item);
     QPoint degrees = event->angleDelta() / 8;
     if (degrees.y() > 0)
     {
         scale(Global::zoomMul,Global::zoomMul);
         currentScale *= Global::zoomMul;
     }
     else
     {
         scale(1/Global::zoomMul,1/Global::zoomMul);
         currentScale /= Global::zoomMul;
     }
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{

}

void ImageView::mousePressEvent(QMouseEvent *event)
{

}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{

}

void ImageView::resizeEvent(QResizeEvent *event)
{
    QRectF rectf = sceneRect();
    fitInView(rectf,Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}















