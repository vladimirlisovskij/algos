#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <qcustomplot-source/qcustomplot.h>
#include "exprtk/exprtk.hpp"
#include "row.h"
#include "indicator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    QCustomPlot *_qplot;
    QCPGraph *_ok_gra, *_not_gra, *_func_gra, *_simpson_gra;
    indicator *_ind;
    QPushButton *_plot_but;
    QLabel *_m_c_result, *_m_c_dev, *_simpson_result;
    row *_func_row, *_left_row, *_right_row, *_up_row, *_down_row, *_epsilon_row, *_dot_row, *_rol_row, *_epoch_row, *_steps_row;
    QRandomGenerator *_gen;
    exprtk::expression<double> *_expression;
    exprtk::parser<double> *_parser;
    double _left, _right, _up, _down, _epsilon;
    double _cur_x, _epsilon_val;
    double _func(double x);
    double _std_dev(const QVector<double>& values);
    double _simpson(double left, double right, qint32  k);
    double _adapt_simpson(double left, double right, qint32  start_k, qint32  steps);
    void _m_c(qint32  n_rolls, qint32 n);
    void _on_click();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
