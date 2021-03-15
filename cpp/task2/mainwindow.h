#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "matrix_widget.h"
#include "row.h"
#include "matrix.h"
#include "indicator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    matrix_widget* _mat;
    indicator *_ind;
    QPushButton *_build_but, *_gauss_but, *_gradient_but, *_cgm_but, *_reg_but;
    row *_col_row, *_eps_row, *_gd_steps_row, *_cgm_eps_row, *_cgm_steps_row, *_reg_eps_row, *_reg_alpha_row, *_reg_steps_row;;
    QLabel *_gauss_lab, *_gradient_lab, *_cgm_lab, *_reg_lab;

    void _build();
    void _gauss();
    void _gd();
    void _cgm();
    void _reg();

public:
    MainWindow(QWidget *parent = nullptr);
};
#endif // MAINWINDOW_H
