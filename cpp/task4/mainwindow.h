#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot.h"
#include "Helper.h"

#include <QMainWindow>
#include <qpair.h>

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    using point = QPair<double, double>;

    Ui::MainWindow *ui;
    QCPGraph *funGra, *dotGra;
    QCPCurve *bezCur;

    void plotSpline(const QVector<double> &nums, double start, double end, qint32 steps, double &minY, double &maxY);
    point bezier(QVector<point> points, double t);
};
#endif // MAINWINDOW_H
