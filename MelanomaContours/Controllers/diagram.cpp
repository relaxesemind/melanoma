#include "diagram.h"
#include "ui_diagram.h"
#include "Models/appstorage.h"
#include "Managers/grapher.h"
#include "Managers/managerslocator.h"
#include <QString>
#include <QFileDialog>

Diagram::Diagram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Diagram)
{
    ui->setupUi(this);
    this->setWindowTitle("Диаграмма");
    updateAverageLabels();

    ui->gridLayout->addWidget(Grapher::shared().view, 1, 0);
    ui->comboBox->addItems({"Длина", "Толщина", "Цвет", "Угол наклона"});
}

Diagram::~Diagram()
{
    delete ui;
}

void Diagram::updateAverageLabels()
{
    auto& storage = AppStorage::shared();
    ui->label->setText("Длина: " + QString::number(storage.averageLenght) + " пикс.");
    ui->label_2->setText("Толщина: " + QString::number(storage.averageThick) + " пикс.");

    QRgb color = storage.averageColor.rgb();
    QString hex = storage.averageColor.name();
    QString r = QString::number(qRed(color));
    QString g = QString::number(qGreen(color));
    QString b = QString::number(qBlue(color));
    QString colorString = "R:" + r + " G:" + g + " B:" + b + " | hex " + hex;
    ui->label_3->setText("Цвет: " + colorString);
    ui->label_4->setText("Угол наклона: " + QString::number(storage.averageAngle) + " град.");
}

void Diagram::on_comboBox_currentIndexChanged(int index)
{
    auto& helper = ManagersLocator::shared().helper;
    int value = index;

    connect(&helper, &Helper::pointsEmitted, this, [&, value](const QVector<QPointF>& points)
    {
        auto& grapher = Grapher::shared();
        grapher.clearView();
        QVector<QPointF> zeros;
        for (int i = 0; i < points.count(); ++i)
        {
            zeros.append(QPointF(points[i].x(), 0));
        }

        QString title;

        switch (value)
        {
        case 0: title = "Длина"; break;
        case 1: title = "Толщина"; break;
        case 2: title = "Цвет"; break;
        case 3: title = "Угол наклона"; break;
        default: title = "unknown"; break;
        }

//        grapher.addGraph(zeros, "Ноль", QColor(Qt::red));
        grapher.addGraph(points, title, QColor(Qt::darkBlue));
    });

    helper.preparePointsForGraph(value);
}

void Diagram::on_pushButton_clicked()
{
    QPixmap pix = Grapher::shared().view->grab();
    QString path = QFileDialog::getSaveFileName(this, "Сохранить изображение",
                                                qApp->applicationDirPath(),
                                                tr("Images (*.png)"));
    if (path.isNull())
    {
        return;
    }

    pix.save(path,"PNG");
}










