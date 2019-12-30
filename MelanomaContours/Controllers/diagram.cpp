#include "diagram.h"
#include "ui_diagram.h"
#include "Models/appstorage.h"
#include "Managers/grapher.h"
#include "Managers/managerslocator.h"
#include <QString>

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
    Grapher::shared().clearView();
//    int factor = 1;
//    while (AppStorage::shared().lines.size() / factor > 300)
//    {
//        ++factor;
//    }

    auto& helper = ManagersLocator::shared().helper;
    connect(&helper, &Helper::pointsEmitted, this, [&](const QVector<QPointF>& points){
        Grapher::shared().addGraph(points,"Дельта",QColor(Qt::darkBlue));
    });

    helper.preparePointsForGraph(index);
}







