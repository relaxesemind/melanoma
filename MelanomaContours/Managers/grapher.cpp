#include "grapher.h"
#include <QPainter>

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

    QSplineSeries *seriesLine = new QSplineSeries();

    for (QPointF p : points)
    {
        seriesLine->append(p);
    }

    QPen pen(lineColor);
    pen.setWidthF(2);

    seriesLine->setPen(pen);
    seriesLine->setName(title);
    chart->addSeries(seriesLine);

    if (chart->series().isEmpty())
    {
        return;
    }

    chart->createDefaultAxes();
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




