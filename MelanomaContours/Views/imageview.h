#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "Views/drawtool.h"
#include "Models/classmodel.h"

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QLineF>
#include <type_traits>
#include <memory>

class ImageView : public QGraphicsView
{
    Q_OBJECT

    using pGraphicsItem = std::shared_ptr<QGraphicsPixmapItem>;

public:
    explicit ImageView(QWidget* widget = nullptr);

    QImage getImage() const;

    void setImage(const QImage &value);

protected:
    void wheelEvent(QWheelEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;


public slots:
    void clearView();
    void updateWithCurrentClass(const ClassModel& model);
    void showAllClasses();

signals:

private: //property
///Main
    QGraphicsScene *scene;
    qreal currentScale;
    QGraphicsPixmapItem *item;
    QImage image;

///DrawingLogic
    bool isReadyToDraw();
    bool drawFlag;
    QPointF previousPoint, startPoint;

    QGraphicsRectItem *tempRect;
    QGraphicsLineItem *tempLine;
    QList<QGraphicsLineItem *> currentLine;
    QRectF currentRect;



private: //methods
    QPointF transformCoordinates(QPointF pos) const;
    QPen currentPen();
    QPolygonF polygonFromLine(QList<QGraphicsLineItem *> line);
    QImage createSubImage(const QImage& image, const QRect & rect);
    QImage createSubImage(const QImage& image, const QPainterPath & path);
};

#endif // IMAGEVIEW_H
