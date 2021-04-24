#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Lagrange.h"
#include "exprtk.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    funGra = ui->graph->addGraph();
    dotGra = ui->graph->addGraph();

    bezCur = new QCPCurve(ui->graph->xAxis, ui->graph->yAxis);

    dotGra->setAdaptiveSampling(false);
    dotGra->setLineStyle(QCPGraph::lsNone);
    dotGra->setScatterStyle(QCPScatterStyle::ssDot);
    dotGra->setPen(QPen(QColor("red"), 5));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
//    7.2 5.9 4.9 4 3.2
    qint32 steps = ui->lagStepBox->text().toInt();
    QStringList xStr = ui->xRow->text().split(" ");
    QVector<double> x;
    foreach (QString str, xStr) {
        x.push_back(str.toDouble());
    }
    QStringList yStr = ui->yRow->text().split(" ");
    QVector<double> y;
    foreach (QString str, yStr) {
        y.push_back(str.toDouble());
    }
    if (x.size() != y.size()) return;
    dotGra->data()->clear();
    funGra->data()->clear();
    bezCur->data()->clear();
    dotGra->setData(x, y);
    Lagrange l (x, y);
    qint32 xS = x.first();
    qint32 xE = x.last();
    double cur = xS;
    double stepVal = (xE - xS) / (double) steps;
    double minY = std::numeric_limits<qint32>::max();
    double maxY = std::numeric_limits<qint32>::min();
    for (int step = 0; step <= steps; ++step){
        double temp = l.getValue(cur);
        minY = qMin(temp, minY);
        maxY = qMax(temp, maxY);
        funGra->addData(cur, temp);
        cur += stepVal;
    }
    ui->graph->xAxis->setRange(xS - 1, xE + 1);
    ui->graph->yAxis->setRange(minY - 1, maxY + 1);
    ui->graph->replot();
}

void MainWindow::plotSpline(const QVector<double> &nums, double start, double end, qint32 steps, double &minY, double &maxY)
{
    double it = (end - start) / (double)steps;
    for (qint32 j = 0; j <= steps; ++j) {
        double temp = Helper::splineValue(nums, start);
        minY = qMin(temp, minY);
        maxY = qMax(temp, maxY);
        funGra->addData(start, temp);
        start += it;
    }
}

MainWindow::point MainWindow::bezier(QVector<MainWindow::point> points, double t)
{
    qint32 i = points.size() - 1;
    while (i > 0) {
           for (int k = 0; k < i; k++) {
                points[k].first = points[k].first + t * (points[k+1].first - points[k].first);
                points[k].second = points[k].second + t * (points[k+1].second - points[k].second);
           }
           i--;
       }
    return points[0];
}

void MainWindow::on_pushButton_2_clicked()
{
    qint32 steps = ui->cSplStepBox->text().toInt();
    qint32 parts = ui->cSplPartBox->text().toInt();

    double start = ui->cSplXBox->text().toDouble();
    double end = ui->cSplYBox->text().toDouble();

    double it = (end - start) / (double)parts;

    exprtk::expression<double> expression;
    exprtk::parser<double> parser;
    exprtk::symbol_table<double> table;
    table.add_variable("x", start);
    table.add_constants();
    expression.register_symbol_table(table);
    parser.compile(ui->cSplFun->text().toStdString() , expression);

//    QVector<QPair<double,double>> points (parts+1);
//    for (qint32 i = 0; i <= parts; ++i) {
//        points[i] = {start, expression.value()};
//        start += it;
//    }

    QVector<QPair<double,double>> points {
        {1, 12},
        {2, 2},
        {3, -1}
    };

    auto res = Helper::getSplines(points);

    dotGra->data()->clear();
    funGra->data()->clear();
    bezCur->data()->clear();
    double minY = std::numeric_limits<qint32>::max();
    double maxY = std::numeric_limits<qint32>::min();
    qint32 nSplines = res.size();
    for (qint32 i = 0; i < nSplines; ++i) {
        dotGra->addData(points[i].first, points[i].second);
        plotSpline(res[i], points[i].first, points[i + 1].first, steps, minY, maxY);
    }
    for (qint32 i = 0; i < nSplines; ++i) {
        dotGra->addData(points[i].first, points[i].second);
    }
    dotGra->addData(points.back().first, points.back().second);
    ui->graph->xAxis->setRange(points.first().first, points.last().first);
    ui->graph->yAxis->setRange(minY, maxY);
    ui->graph->replot();
}

void MainWindow::on_pushButton_3_clicked()
{
    qint32 parts = ui->cSplStepBox->text().toInt();
    qint32 steps = ui->cSplPartBox->text().toInt();

    double startX = ui->cSplXBox->text().toDouble();
    double endX = ui->cSplYBox->text().toDouble();

    double it = (endX - startX) / (double)steps;

    double xVal;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;
    exprtk::symbol_table<double> table;
    table.add_variable("x", xVal);
    table.add_constants();
    expression.register_symbol_table(table);
    parser.compile(ui->cSplFun->text().toStdString() , expression);

//    QVector<double> x(steps + 1);
//    double temp = startX;
//    for (int i = 0; i <= steps; ++i) {
//        x[i] = temp;
//        temp += it;
//    }

//    QVector<double> y = x;
//    for (int i = 0; i <= steps; ++i) {
//        xVal = y[i];
//        y[i] = expression.value();
//    }

    steps = 2;
    QVector<double> x = {1, 2, 3};
    QVector<double> y = {12, 2, -1};

    dotGra->data()->clear();
    funGra->data()->clear();
    bezCur->data()->clear();

    dotGra->setData(x, y);

    QVector<double> der = y;
    for (int i = 1; i < steps; ++i) {
        der[i] =
                      (y[i + 1] - y[i])
            / // -----------------------------
                    (2 * (x[i + 1] - x[i]))
            +
                      (y[i] - y[i - 1])
            / // -----------------------------
                    (2 * (x[i] - x[i - 1]));

    }
    der[0] =
                      (y[1] - y[0])
            / // -----------------------------
                    (2 * (x[1] - x[0]));

    der[steps] =
                        (y[steps] - y[steps - 1])
            / // ------------------------------------
                    (2 * (x[steps] - x[steps - 1]));

    double minY = std::numeric_limits<qint32>::max();
    double maxY = std::numeric_limits<qint32>::min();

    double sX = startX;
    double eX = startX + it;
    double it2 = (eX - sX) / (double) parts;
    for (int i = 0; i < steps; ++i) {
        double pk = y[i];
        double pk1 = y[i +1];
        double mk = der[i];
        double mk1 = der[i + 1];
        double tt = sX;
        for (int j = 0; j <= parts; ++j) {
            double t = (tt - sX)/it;
            double point =
                    (2*t*t*t - 3*t*t + 1)*pk +
                    (t*t*t - 2*t*t + t)*mk +
                    (-2*t*t*t + 3*t*t)*pk1 +
                    (t*t*t - t*t)*mk1;
            funGra->addData(tt, point);
            minY = qMin(point, minY);
            maxY = qMax(point, maxY);
            tt += it2;
        }
        sX = eX;
        eX += it;
    }
    ui->graph->yAxis->setRange(minY, maxY);
    ui->graph->xAxis->setRange(startX, endX);
    ui->graph->replot();
}

void MainWindow::on_pushButton_4_clicked()
{
    qint32 steps = ui->bezStepBox->text().toInt();
    steps = 4;
    QStringList xStr = ui->bezxRow->text().split(" ");
    QVector<double> x;
    foreach (QString str, xStr) {
        x.push_back(str.toDouble());
    }
    QStringList yStr = ui->bezyRow->text().split(" ");
    QVector<double> y;
    foreach (QString str, yStr) {
        y.push_back(str.toDouble());
    }
    if (x.size() != y.size()) return;
    dotGra->data()->clear();
    funGra->data()->clear();
    bezCur->data()->clear();
    qint32 size = x.size();

    QVector<point> points (size);

    for (qint32 i = 0; i < size; ++i) {
        points[i] = {x[i], y[i]};
    }

    double minY = std::numeric_limits<qint32>::max();
    double maxY = std::numeric_limits<qint32>::min();
    double minX = std::numeric_limits<qint32>::max();
    double maxX = std::numeric_limits<qint32>::min();
    for (auto i: points) {
        dotGra->addData(i.first, i.second);
        minY = qMin(i.second, minY);
        maxY = qMax(i.second, maxY);
        minX = qMin(i.first, minX);
        maxX = qMax(i.first, maxX);
    }
    double it = 1/(double)steps;
    double t = 0;
    for (qint32 i = 0; i <= steps; ++i) {
        point temp = bezier(points, t);
        bezCur->addData(temp.first, temp.second);
        t += it;
    }
    ui->graph->yAxis->setRange(minY, maxY);
    ui->graph->xAxis->setRange(minX, maxX);
    ui->graph->replot();
}
