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
    QGraphicsPixmapItem *item;
    QImage image;

private: //methods
    QPointF transformCoordinates(QPointF pos) const;
};

#endif // IMAGEVIEW_H
