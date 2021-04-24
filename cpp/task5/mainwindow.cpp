#include "FunctionHelper.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    funGra = ui->graph->addGraph();
    fourGra = ui->graph->addGraph();

    fourGra->setPen(QPen(QColor("red")));

}

MainWindow::~MainWindow()
{
    delete ui;
}

double atPerio(double left, double right, double x){
    double p = right - left;
    if (x > left) {
        qint32 t = (x - left) / p;
        return x - p*t;
    } else {
        qint32 t = (right - x) / p;
        return x + p*t;
    }
}

void MainWindow::on_pushButton_clicked()
{
    double minY = std::numeric_limits<qint32>::max();
    double maxY = std::numeric_limits<qint32>::min();

    funGra->data()->clear();
    fourGra->data()->clear();

    double per = ui->period->text().toDouble();
    double l = ui->left->text().toDouble();
    double r = ui->right->text().toDouble();

    string fun = ui->funcLine->text().toStdString();

    Fourier f (fun, -per, per, ui->nodes->text().toInt(), ui->steps->text().toInt());

    double step = (r - l) / 1000.0;
    double start = l;
    for (qint32 i = 0; i < 1000; ++i) {
        double y = f.value(start);
        minY = qMin(minY, y);
        maxY = qMax(maxY, y);

        fourGra->addData(start, y);
        funGra->addData(start, f.rawVal(atPerio(-per, per, start)));

        start += step;
    }
    ui->graph->yAxis->setRange(minY, maxY);
    ui->graph->xAxis->setRange(l, r);
    ui->graph->replot();
}
