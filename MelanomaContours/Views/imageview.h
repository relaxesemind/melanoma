#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "Views/drawtool.h"

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
    void setOpacity(qreal value);
    void setOverlayImage(const QImage &value);
    QImage getOverlayImage() const;
    void drawSectors(QPoint areaCenter, qreal mainRadius, int numberOfRadius = 4, int numberOfSectors = 4);

protected:
    void wheelEvent(QWheelEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;


public slots:
    void clearView();

signals:

private: //property
///Main
    QGraphicsScene *scene;
    qreal currentScale;
    qreal opacity;
    QGraphicsPixmapItem *item, *overlayItem;
    QImage image, overlayImage;

private: //methods
    QPointF transformCoordinates(QPointF pos) const;
    QVector<QGraphicsEllipseItem *> circles;
    QVector<QGraphicsLineItem *> radiuses;
    void removeCircles();
    void removeRadiuses();
    QGraphicsEllipseItem *addCircleToScene(QPoint center, qreal radius);
    QGraphicsLineItem *addRadiusToScene(QPoint center, qreal radius, qreal angle);
};

#endif // IMAGEVIEW_H
