#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "exprtk.hpp"
#include "qcustomplot.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_graBut_clicked();

    void on_dihBut_clicked();

private:
    Ui::MainWindow *ui;
    double funcValue(double x);
    void dihotomy(double left, double right, double dihEps, double secEps, QString& result);
    void secand(double left, double right, double eps, QString& result);

    std::unique_ptr<exprtk::expression<double>> _expression;
    std::unique_ptr<exprtk::parser<double>> _parser;
    QCPGraph *_funcGra, *_ansGra;
    double _curX;
};
#endif // MAINWINDOW_H
