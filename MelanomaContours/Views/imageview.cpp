#include "imageview.h"
#include <QDebug>
#include "Common/consts.h"
#include "Models/appstorage.h"
#include <QtMath>
#include "Common/magic.h"
#include "Models/polyareamodel.h"
#include "Models/rectareamodel.h"

ImageView::ImageView(QWidget *widget) : QGraphicsView(widget)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setAlignment(Qt::AlignCenter);
    currentScale = 1.0;
    drawFlag = false;
    tempRect = nullptr;
    tempLine = nullptr;
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

void ImageView::clearView()
{
    scene->clear();
    item = nullptr;
    tempRect = nullptr;
    tempLine = nullptr;
    currentLine.clear();
}

void ImageView::updateWithCurrentClass(const ClassModel &model)
{
    clearView();
    setImage(image);

    for (pBaseAreaModel area : model.areas)
    {
         QPolygonF poly(area->points());

        for_magic(it, poly)
        {
            auto next = std::next(it);
            if (next == poly.end())
            {
                next = poly.begin();
                scene->addLine(QLineF(*it,*next),currentPen());
                break;
            }

            scene->addLine(QLineF(*it,*next),currentPen());
        }

    }
}

void ImageView::showAllClasses()
{
    clearView();
    setImage(image);

    auto& models = AppStorage::shared().classModelsVector;

    for (ClassModel &model : models)
    {
        for (pBaseAreaModel area : model.areas)
        {
             QPolygonF poly(area->points());

            for_magic(it, poly)
            {
                auto next = std::next(it);
                QPen pen = QPen(QBrush(model.color),3,Qt::SolidLine,Qt::RoundCap);
                if (next == poly.end())
                {
                    next = poly.begin();
                    scene->addLine(QLineF(*it,*next), pen);
                    break;
                }

                scene->addLine(QLineF(*it,*next), pen);
            }

        }
    }
}

bool ImageView::isReadyToDraw()
{
    return (!scene->items().isEmpty() and !AppStorage::shared().classModelsVector.isEmpty());
}

QPointF ImageView::transformCoordinates(QPointF pos) const
{
    return mapToScene(pos.x(),pos.y());
}

QPen ImageView::currentPen()
{
    auto& current = AppStorage::shared().currentModel();
    return QPen(QBrush(current.color),3,Qt::SolidLine,Qt::RoundCap);
}

QPolygonF ImageView::polygonFromLine(QList<QGraphicsLineItem *> completeLine)
{
    QVector<QPointF> points;
    for_magic(line, completeLine)
    {
        QLineF qline = (*line)->line();
        QPointF startPoint = qline.p1();
        points << startPoint;
    }
    return QPolygonF(points);
}

QImage ImageView::createSubImage(const QImage &image, const QRect &rect)
{
    return image.copy(rect);
}

QImage ImageView::createSubImage(const QImage &input, const QPainterPath &path)
{
    if(!input.isNull() and !path.isEmpty())
    {
        QRect r = path.boundingRect().toRect().intersected(input.rect());
        QImage tmp(input.size(), QImage::Format_ARGB32);
        tmp.fill(Qt::transparent);

        QPainter painter(&tmp);
        painter.setClipPath(path);
        painter.drawImage(QPoint{}, input, input.rect());
        painter.end();

        return tmp.copy(r);
    }
    return QImage();
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
    if (!drawFlag or !isReadyToDraw())
    {
        return;
    }

    QPointF point = transformCoordinates(event->pos());

//    if (image.rect().contains(point.toPoint()))
//    {
//        return;
//    }

    switch (AppStorage::shared().drawTool)
    {
        case DrawTool::Rect:
        {
             if (tempRect and tempRect->scene() == scene)
             {
                 scene->removeItem(tempRect);
                 tempRect = nullptr;
             }

             QRectF rect;
             rect.setBottomLeft(previousPoint);
             rect.setTopRight(point);
             currentRect = rect.normalized();

             tempRect = scene->addRect(rect,currentPen());
             this->update();
        break;
        }
         case DrawTool::Curve:
        {
            QLineF line(previousPoint, point);
            currentLine << scene->addLine(line,currentPen());
            previousPoint = point;
        break;
        }
    }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (!isReadyToDraw())
    {
        return;
    }

    currentLine.clear();
    drawFlag = true;
    previousPoint = transformCoordinates(event->pos());
    startPoint = previousPoint;
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    drawFlag = false;
    if (!isReadyToDraw())
    {
        return;
    }

    ClassModel& currentModel = AppStorage::shared().currentModel();
    DrawTool tool = AppStorage::shared().drawTool;

    if (tool == DrawTool::Rect)
    {
        if (!image.rect().contains(currentRect.toRect()))
        {
            return;
        }

        scene->addRect(currentRect,currentPen());

        if (tempRect and tempRect->scene() == scene)
        {
            scene->removeItem(tempRect);
            tempRect = nullptr;
        }

        QImage piece = createSubImage(image,currentRect.toRect());

        pointer_magic(RectAreaModel, rectModel);
        rectModel->rect = currentRect;
        rectModel->imageArea = piece;
        currentModel.areas.append(rectModel);

//        setImage(piece);
    }

    if (tool == DrawTool::Curve)
    {
        QPointF point = transformCoordinates(event->pos());
        currentLine << scene->addLine({point,startPoint},currentPen());

        QPainterPath path;
        QPolygonF poly = polygonFromLine(currentLine);
        path.addPolygon(poly);
        QImage piece = createSubImage(image,path);

        pointer_magic(PolyAreaModel, polyModel);
        polyModel->polygon = poly;
        polyModel->imageArea = piece;
        currentModel.areas.append(polyModel);

//        setImage(piece);
    }
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    QRectF rectf = sceneRect();
    fitInView(rectf,Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}















