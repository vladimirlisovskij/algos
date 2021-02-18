#include "mainwindow.h"
#include <QtMath>

double MainWindow::_func(double x)
{
    _cur_x = x;
    return _expression->value();
}

double MainWindow::_std_dev(const QVector<double> &values)
{
    int size = values.size();
    double left = 0, right = 0;
    for (int i = 0; i < size; ++i){
        double val = values[i];
        left += val * val;
        right += val;
    }
    left /= size;
    right /= size;
    right *= right;
    return qSqrt(left - right);
}

double MainWindow::_simson(double left, double right, int k)
{
    QVector<double> vals(k);
    double step = (right - left) / (double)k;
    double cur_step = left;
    for (double& i: vals) {
        i = _func(cur_step);
        cur_step += step;
    }
    double res1 = (right - left) / 3.0 / k;
    double res2 = vals.front() + vals.back();
    double temp = 0;
    for (int i = 1; i < k - 2; i += 2) {
        temp += vals[i];
    }
    res2 += 4 * temp;
    temp = 0;
    for (int i = 2; i < k - 3; i += 2) {
        temp += vals[i];
    }
    res2 += 2 * temp;
    return res1 * res2;
}

double MainWindow::_adapt_simson(double left, double right, int start_k, int steps)
{
    double ans = _simson(left, right, start_k);
    start_k <<= 1;
    double ans2 = _simson(left, right, start_k);
    if (qAbs(ans - ans2) > _epsilon_val && steps) {
        double middle = (left + right) / 2.0;
        return _adapt_simson(left, middle, start_k, steps-1) + _adapt_simson(middle, right, start_k, steps-1);
    }
    return ans2;
}

void MainWindow::_m_c(int n_rolls, int val)
{
    QVector<double> ok_x, ok_y, not_x, not_y, func_x, func_y;
    double lenght = _right - _left;
    double height = _up - _down;
    int counter = 0;
    for (int i = 0; i < n_rolls; ++i) {
        double x = _left + _gen->generateDouble() * lenght;
        double y = _down + _gen->generateDouble() * height;
        double true_y = _func(x);
        if (true_y >= 0) {
            if (true_y >= y && y >= 0) {
                ok_x.append(x);
                ok_y.append(y);
                counter++;
            } else {
                not_x.append(x);
                not_y.append(y);
            }
        } else {
            if (true_y <= y && y <= 0) {
                ok_x.append(x);
                ok_y.append(y);
                counter--;
            } else {
                not_x.append(x);
                not_y.append(y);
            }
        }
    }
    double step = lenght / (double) val;
    double start = _left;
    for (qint32 x = 0; x < val; ++x){
        func_x.append(start);
        func_y.append(_func(start));
        start += step;
    }
    _m_c_result->setText(QString("MC resut: ") + QString::number(height * lenght * counter / (double)n_rolls, 'g', 10));
    _m_c_dev->setText(QString("MC dev: ") + QString::number(_std_dev(ok_y)));
    _ok_gra->setData(ok_x, ok_y);
    _ok_gra->setPen(QPen(QColor("green")));
    _not_gra->setData(not_x, not_y);
    _not_gra->setPen(QPen(QColor("red")));
    _func_gra->setData(func_x, func_y);
    _qplot->replot();
}

void MainWindow::_on_click()
{
    QString raw_func = _func_row->text();
    _parser->compile(raw_func.toStdString(), *_expression);
    _left = _left_row->text().toDouble();
    _right = _right_row->text().toDouble();
    _up = _up_row->text().toDouble();
    _down = _down_row->text().toDouble();
    _epsilon_val = qPow(10, _epsilon_row->text().toInt());
    _qplot->xAxis->setRange(_left, _right);
    _qplot->yAxis->setRange(_down, _up);
    _simson_result->setText(QString("Simson resut: ") + QString::number(_adapt_simson(_left, _right, 2, 10), 'g', 10));
    _m_c(_rol_row->text().toInt(), _dot_row->text().toInt());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _qplot(new QCustomPlot())
    , _ok_gra(_qplot->addGraph())
    , _not_gra(_qplot->addGraph())
    , _func_gra(_qplot->addGraph())
    , _plot_but(new QPushButton("Построить"))
    , _m_c_result(new QLabel())
    , _m_c_dev(new QLabel())
    , _simson_result(new QLabel())
    , _func_row(new row("функция"))
    , _left_row(new row("левая граница"))
    , _right_row(new row("правая граница"))
    , _up_row(new row("верхняя граница"))
    , _down_row(new row("нижняя граница"))
    , _epsilon_row(new row("точность"))
    , _dot_row(new row("число точек"))
    , _rol_row(new row("число бросков"))
    , _steps_row(new row("число шагов рекурсии"))
    , _main(new QVBoxLayout())
    , _gen(new QRandomGenerator())
    , _expression(new exprtk::expression<double>())
    , _parser(new exprtk::parser<double>())
{
    _qplot->setFixedSize(300, 300);
    _ok_gra->setAdaptiveSampling(false);
    _ok_gra->setLineStyle(QCPGraph::lsNone);
    _ok_gra->setScatterStyle(QCPScatterStyle::ssCircle);
    _not_gra->setAdaptiveSampling(false);
    _not_gra->setLineStyle(QCPGraph::lsNone);
    _not_gra->setScatterStyle(QCPScatterStyle::ssCircle);
    exprtk::symbol_table<double> table;
    table.add_variable("x", _cur_x);
    table.add_constants();
    _expression->register_symbol_table(table);
    QLocale lo(QLocale::C);
    lo.setNumberOptions(QLocale::RejectGroupSeparator);
    QDoubleValidator* val = new QDoubleValidator(this);
    val->setLocale(lo);
    _main->addWidget(_qplot);
    _main->addWidget(_plot_but);
    _main->addWidget(_m_c_result);
    _main->addWidget(_m_c_dev);
    _main->addWidget(_simson_result);
    _main->addWidget(_func_row);
    _main->addWidget(_left_row);
    _left_row->setValidator(val);
    _main->addWidget(_right_row);
    _right_row->setValidator(val);
    _main->addWidget(_down_row);
    _down_row->setValidator(val);
    _main->addWidget(_up_row);
    _up_row->setValidator(val);
    _main->addWidget(_dot_row);
    _dot_row->setValidator(val);
    _main->addWidget(_rol_row);
    _rol_row->setValidator(new QIntValidator());
    _main->addWidget(_epsilon_row);
    _epsilon_row->setValidator(new QIntValidator());
    _main->addWidget(_steps_row);
    _steps_row->setValidator(new QIntValidator());
    _main->setSpacing(0);
    _main->setContentsMargins(0, 0, 0, 0);
    QWidget *main_wig = new QWidget();
    main_wig->setLayout(_main);
    this->setCentralWidget(main_wig);
    connect(_plot_but, &QPushButton::clicked, this, &MainWindow::_on_click);
    this->setFixedSize(300, 800);
}

MainWindow::~MainWindow()
{
    delete _gen;
    delete _expression;
    delete _parser;
}

