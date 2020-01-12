#include "grapher.h"
#include <QPainter>
#include <QLegendMarker>

Grapher::Grapher()
{
    view = new ChartView();
    chart = new QChart();

    view->setChart(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRubberBand(QChartView::RectangleRubberBand);

    updateState();
}

void Grapher::updateState()
{
    chart->legend()->hide();
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Отклонения от средних величин");

//    chart->setAnimationOptions(QChart::SeriesAnimations);
}

void Grapher::addGraph(const QVector<QPointF> &points, QString const& title, QColor lineColor)
{
    chart->legend()->show();
    chart->legend()->setAlignment(Qt::AlignRight);

    QLineSeries *series = new QLineSeries();
    QScatterSeries *dotsSeries = new QScatterSeries();

    series->append(points.toList());
    dotsSeries->append(points.toList());

    QPen pen1(lineColor);
    pen1.setWidthF(2);

    QPen pen2(Qt::red);
    pen2.setWidth(3);

    series->setPen(pen1);
    series->setName(title);
//    dotsSeries->setPen(pen2);
    dotsSeries->setBorderColor(Qt::red);
    dotsSeries->setMarkerSize(4);
//    dotsSeries->set


    chart->addSeries(series);
    chart->addSeries(dotsSeries);

    if (chart->series().isEmpty())
    {
        return;
    }

    chart->createDefaultAxes();
    chart->legend()->markers(dotsSeries)[0]->setVisible(false);
}

void Grapher::clearView()
{
    chart->removeAllSeries();
}

void Grapher::setXRange(float min, float max)
{
    minX = min;
    maxX = max;
}

void Grapher::setYRange(float min, float max)
{
    minY = min;
    maxY = max;
}

bool Grapher::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == rubberBand && event->type() == QEvent::Resize)
    {
            QPointF fp = chart->mapToValue(rubberBand->geometry().topLeft());
            QPointF tp = chart->mapToValue(rubberBand->geometry().bottomRight());
            emit rubberBandChanged(fp, tp);
    }

    return QObject::eventFilter(watched, event);
}

void Grapher::rubberZoomAdapt(QPointF fp, QPointF tp)
{
  // qDebug() << "zoom" << fp << " " << tp;
}




