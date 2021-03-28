#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _expression(new exprtk::expression<double>())
    , _parser(new exprtk::parser<double>())
{
    ui->setupUi(this);
    exprtk::symbol_table<double> table;
    table.add_variable("x", _curX);
    table.add_constants();
    _expression->register_symbol_table(table);
    _ansGra = ui->graph->addGraph();
    _ansGra->setScatterStyle(QCPScatterStyle::ssDot);
    _ansGra->setLineStyle(QCPGraph::lsNone);
    _ansGra->setAdaptiveSampling(false);
    _ansGra->setPen(QPen(QColor("red"), 5));
    _funcGra = ui->graph->addGraph();
}

MainWindow::~MainWindow() {
    delete ui;
}


double MainWindow::funcValue(double x) {
    _curX = x;
    return _expression->value();
}

void MainWindow::dihotomy(double left, double right, double dihEps, double secEps, QString &result) {
    double center = (left + right) / 2.0;
    double centerVal = funcValue(center);
    if (qAbs(centerVal) <= dihEps){
        secand(left, right, secEps, result);
    } else {
        double leftVal = funcValue(left);
        double rightVal = funcValue(right);
        if ( leftVal * centerVal < 0) dihotomy(left, center, dihEps, secEps, result);
        if ( rightVal * centerVal < 0) dihotomy(center, right, dihEps, secEps, result);
    }
}

void MainWindow::secand(double curX, double prevX, double eps, QString &result) {

     while (qAbs(curX - prevX) > eps) {
        double temp = curX;
        curX -=
                (curX - prevX) * funcValue(curX)
            / //-----------------------------------
                (funcValue(curX) - funcValue(prevX));
        prevX = temp;
    }
    result += QString::number(curX) + QString(" ");
}

void MainWindow::on_graBut_clicked() {
    _parser->compile(ui->funcLine->text().toStdString(), *_expression);
    _ansGra->data()->clear();
    _funcGra->data()->clear();
    double left = ui->leftBox->text().toDouble();
    double right = ui->rightBox->text().toDouble();
    if (left > right) return;
    double eps = qPow(10.0, ui->graEpsBox->text().toInt());
    double minY = std::numeric_limits<qint32>::max();
    double maxY = std::numeric_limits<qint32>::min();
    qint32 nSteps = ui->graStepsBox->text().toInt();
    double step = (right - left) / (double) nSteps;
    double x = left;
    for (qint32 i = 0; i <= nSteps; ++i) {
        double y = funcValue(x);
        minY = qMin(y, minY);
        maxY = qMax(y, maxY);
        _funcGra->addData(x, y);
        if (qAbs(y) <= eps) _ansGra->addData(x, y);
        x += step;
    }
    ui->graph->yAxis->setRange(minY, maxY);
    ui->graph->xAxis->setRange(left, right);
    ui->graph->replot();
}

void MainWindow::on_dihBut_clicked() {
    _parser->compile(ui->funcLine->text().toStdString(), *_expression);
    double left = ui->leftBox->text().toDouble();
    double right = ui->rightBox->text().toDouble();
    if (left > right) return;
    double dihEps = qPow(10.0, ui->dihEpsBox->text().toInt());
    double secEps = qPow(10.0, ui->secEpsBox->text().toInt());
    QString result = "";
    dihotomy(left, right, dihEps, secEps, result);
    ui->dihResLab->setText(result);
}
