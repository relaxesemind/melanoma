#ifndef GRAPHER_H
#define GRAPHER_H

#include "Common/magic.h"

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QtCharts/QChartView>
#include <QEvent>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QDebug>
#include <QRubberBand>
#include <QtCharts/QValueAxis>
#include <QtCharts/QScatterSeries>


class GrapherMode
 {
 public:
    enum Option
    {
        BarVisible = 1, // 000001
        SplineVisible = 2 // 000010
    };

 Q_DECLARE_FLAGS(Options, Option)
 };

Q_DECLARE_OPERATORS_FOR_FLAGS(GrapherMode::Options)

using namespace QtCharts;

class ChartView : public QChartView
{
public:
    ChartView() = default;

//protected:
//    bool viewportEvent(QEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void keyPressEvent(QKeyEvent *event);
};


class Grapher : public QObject
{
    Q_OBJECT
private:
    Grapher();
    Grapher(const Grapher&);
    Grapher operator=(Grapher&);
public:
    static Grapher& shared() {
        static Grapher service;
        return service;
    }

    ChartView *view;
    QChart *chart;

public:
    void updateState();
    void addGraph(QVector<QPointF> const& points, const QString &title, QColor lineColor = QColor());
    void clearView();
    void setXRange(float min, float max);
    void setYRange(float min, float max);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void rubberBandChanged(QPointF fp, QPointF tp);

public slots:
    void rubberZoomAdapt(QPointF fp, QPointF tp);

private:
    float minX,maxX;
    float minY,maxY;
    QRubberBand *rubberBand;
//    QStackedBarSeries *stackSeria;
};

#endif // GRAPHER_H
