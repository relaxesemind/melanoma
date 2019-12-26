#include "imageview.h"
#include <QDebug>
#include "Common/consts.h"
#include "Models/appstorage.h"
#include <QtMath>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include "Common/magic.h"

ImageView::ImageView(QWidget *widget) : QGraphicsView(widget)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setAlignment(Qt::AlignCenter);
    currentScale = 1.0;
    opacity = 1.0;
    item = nullptr;
    overlayItem = nullptr;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet(QString("background-color: #89AFD1;"));
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
    overlayItem = nullptr;
}

QImage ImageView::getOverlayImage() const
{
    return overlayImage;
}

void ImageView::drawSectors(QPoint areaCenter, qreal mainRadius, int numberOfRadius, int numberOfSectors)
{
    if (numberOfRadius < 1 || numberOfSectors < 2) return;
    removeCircles();
    removeRadiuses();

    qreal maxR = mainRadius;
    qreal stepR = maxR / numberOfRadius;
    qreal stepAngle = 2. * M_PI / numberOfSectors;
    for (int i = 0; i < numberOfRadius; ++i)
    {
        qreal r = stepR * (1 + i);
        addCircleToScene(areaCenter, r);
        AppStorage::shared().radiuses.append(r);
    }
    for (int i = 0; i < numberOfSectors; ++i)
    {
        qreal angle = stepAngle * (1 + i);
        addRadiusToScene(areaCenter, mainRadius, angle);
        AppStorage::shared().angles.append(angle);
    }
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

void ImageView::removeCircles()
{
    for (int i = 0; i < circles.count(); ++i)
    {
        scene->removeItem(circles[i]);
    }

    circles.clear();
    AppStorage::shared().radiuses.clear();
}

void ImageView::removeRadiuses()
{
    for (int i = 0; i < radiuses.count(); ++i)
    {
        scene->removeItem(radiuses[i]);
    }

    radiuses.clear();
    AppStorage::shared().angles.clear();
}

QGraphicsEllipseItem *ImageView::addCircleToScene(QPoint center, qreal radius)
{
    QPen pen((QColor(Qt::red)));
    pen.setWidth(2);
    QPoint topLeft(center.x() - radius, center.y() - radius);
    QPoint bottomRight(center.x() + radius, center.y() + radius);

    QRectF rect(topLeft, bottomRight);
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(rect, this->item);
    item->setPen(pen);

//    scene->addItem(item);
    circles.push_back(item);
    return item;
}

QGraphicsLineItem *ImageView::addRadiusToScene(QPoint center, qreal radius, qreal angle)
{
    qreal x2 = radius * std::cos(angle);
    qreal y2 = radius * std::sin(angle);

    QGraphicsLineItem *item = new QGraphicsLineItem(center.x(), center.y(), x2 + center.x(), y2 + center.y(), this->item);
    QPen pen((QColor(Qt::red)));
    pen.setWidth(2);

    item->setPen(pen);
//    scene->addItem(item);
    radiuses.push_back(item);
    return item;
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

bool pressed = false;
int start_x = 0;
int start_y = 0;

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if (pressed)
    {
        QPointF position = event->pos() - QPoint(start_x, start_y);
        if (item)
        {
           item->setPos(position);
        }
        if (overlayItem)
        {
           overlayItem->setPos(position);
        }
    }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        pressed = true;
        start_x = event->x();
        start_y = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        pressed = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }

    event->ignore();
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    QRectF rectf = sceneRect();
    fitInView(rectf,Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}















