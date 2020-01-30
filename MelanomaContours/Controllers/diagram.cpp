#include "diagram.h"
#include "ui_diagram.h"
#include "Models/appstorage.h"
#include "Managers/grapher.h"
#include "Managers/managerslocator.h"
#include "Common/magic.h"
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QStringList>

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

void Diagram::updateGraph(int index)
{
    auto& helper = ManagersLocator::shared().helper;
    int value = index;

    connect(&helper, &Helper::pointsEmitted, this, [&, value](const QVector<QVector<QPointF>>& points)
    {
        auto& grapher = Grapher::shared();
        grapher.clearView();


        auto& storage = AppStorage::shared();
        QVector<qreal> Karray;

        for (int i = 0; i < storage.V.size(); ++i)
        {
            if (storage.V[i].size() == 0)
            {
                continue;
            }

            qreal Vmax = *std::max_element(storage.V[i].begin(), storage.V[i].end());
            qreal Vmin = *std::min_element(storage.V[i].begin(), storage.V[i].end());

            qreal sum = 0;
            for (int j = 0; j < storage.V[i].count(); ++j)
            {
                sum += storage.V[i][j];
            }
            qreal Vavg = sum / storage.V[i].size();
            if (Vavg == 0)
            {
                continue;
            }

            qreal k = (Vmax - Vmin) / Vavg;
            Karray.append(k);
        }

        qreal Kavg = 1;

        if (Karray.size() != 0)
        {
            qreal sum = 0;
            for (int i = 0; i <Karray.size(); ++i)
            {
                sum += Karray[i];
            }
             Kavg = sum / Karray.size();
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

        if (value != 2)
        {
            grapher.chart->setTitle("Абсолютные величины " + title + " K = " + QString::number(Kavg));
        }
        else
        {
            grapher.chart->setTitle("Расстояние от среднего цвета RGB K = " + QString::number(Kavg));
        }

        const int count = AppStorage::shared().numberOfRadius;
        for (int i = 0; i < count; ++i)
        {
            QVector<QPointF> localPoints = points.at(i);
            QString name = title + " " + QString::number(i + 1) + " Кольцо ";
            QColor rndColor = ManagersLocator::shared().colorGenerator.get();
            grapher.addGraph(localPoints, name, rndColor);

        }
    });

    helper.preparePointsForGraph(value, AppStorage::shared().oXaxisMode);
}

void Diagram::on_comboBox_currentIndexChanged(int index)
{
    updateGraph(index);
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

void Diagram::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить таблицу .csv"),
                                                    qApp->applicationDirPath(),
                                                    tr("csv files (*.csv)"));
    if (fileName.isEmpty())
    {
        return;
    }

    auto& sectors = AppStorage::shared().sectors;

    auto findSector = [&sectors](int lineId)->std::pair<int, int>
    {
        for (int i = 0; i < sectors.count(); ++i)
        {
            Sector& sector = sectors[i];
            for (int j = 0; j < sector.linesIds.count(); ++j)
            {
                if (contains_magic(sector.linesIds, lineId))
                {
                    return sector.getRadSec();
                }
            }
        }

        return std::make_pair(0,0);
    };

    auto csvLine = [](QVector<QString> list)->QString
    {
        QString string;
        for (int i = 0; i < list.count(); ++i)
        {
            string += list.at(i) + ",";
        }

        string += "\n";

        return string;
    };

    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Append))
    {
        QTextStream stream(&file);
        stream << "N,x,y,lenght,width,color,angle,circle,sector \n";

        auto& lines = AppStorage::shared().lines;
        for (int i = 0; i < lines.count(); ++i)
        {
            S_area& line = lines[i];
            auto sector = findSector(line.id);

            stream << csvLine({
                               QString::number(i),
                               QString::number(line.getCenter().x()),
                               QString::number(line.getCenter().y()),
                               QString::number(line.getLenght(),'f', 4),
                               QString::number(line.thickness,'f', 4),
                               line.color.name(),
                               QString::number(line.getAngle(),'f', 4),
                               QString::number(sector.first),
                               QString::number(sector.second)
                              });
        }
    }

    file.close();
}

void Diagram::on_checkBox_clicked(bool checked)
{
    AppStorage::shared().oXaxisMode = checked;
    updateGraph(ui->comboBox->currentIndex());
}
