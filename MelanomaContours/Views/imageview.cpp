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
    overlayItem = nullptr;
}

QImage ImageView::getOverlayImage() const
{
    return overlayImage;
}

void ImageView::drawSectors(QPoint areaCenter, qreal mainRadius, int numberOfRadius, int numberOfSectors)
{
    if (numberOfRadius < 1 || numberOfSectors < 1) return;
    removeCircles();
    removeRadiuses();

    qreal maxR = mainRadius;
    qreal stepR = maxR / numberOfRadius;
    qreal stepAngle = M_PI * 2. / numberOfSectors;
    for (int i = 0; i < numberOfRadius; ++i)
    {
        addCircleToScene(areaCenter, stepR * (1 + i));
    }
    for (int i = 0; i < numberOfSectors; ++i)
    {
        addRadiusToScene(areaCenter, mainRadius, stepAngle * (1 + i));
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
}

void ImageView::removeRadiuses()
{
    for (int i = 0; i < radiuses.count(); ++i)
    {
        scene->removeItem(radiuses[i]);
    }

    radiuses.clear();
}

QGraphicsEllipseItem *ImageView::addCircleToScene(QPoint center, qreal radius)
{
    QPen pen((QColor(Qt::red)));
    pen.setWidth(3);
    QPoint topLeft(center.x() - radius, center.y() - radius);
    QPoint bottomRight(center.x() + radius, center.y() + radius);

    QRectF rect(topLeft, bottomRight);
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(rect);
    item->setPen(pen);

    scene->addItem(item);
    circles.push_back(item);
    return item;
}

QGraphicsLineItem *ImageView::addRadiusToScene(QPoint center, qreal radius, qreal angle)
{
    qreal x2 = radius * std::cos(angle);
    qreal y2 = radius * std::sin(angle);

    QGraphicsLineItem *item = new QGraphicsLineItem(center.x(), center.y(), x2 + center.x(), y2 + center.y());
    QPen pen((QColor(Qt::red)));
    pen.setWidth(3);

    item->setPen(pen);
    scene->addItem(item);
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
        item->setPos(event->pos() - QPoint(start_x, start_y));
        overlayItem->setPos(event->pos() - QPoint(start_x, start_y));
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















