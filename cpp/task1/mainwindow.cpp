#include "mainwindow.h"
#include <QtMath>

double MainWindow::_func(double x) {
    _cur_x = x;
    return _expression->value();
}

double MainWindow::_std_dev(const QVector<double> &values) {
    qint32 size = values.size();
        double left = 0, right = 0;
        for (qint32 i = 0; i < size; ++i){
            double val = values[i];
            left += val * val;
            right += val;
        }
        left /= size;
        right /= size;
        right *= right;
        return qSqrt(left - right);
}

double MainWindow::_simpson(double left, double right, qint32 k) {
    QVector<double> vals(k + 1);
    double step = (right - left) / k;
    double cur_step = left;
    for (qint32 i = 0; i <= k; ++i) {
        vals[i] = _func(cur_step);
        cur_step += step;
    }
    double res1 = (right - left) / (3 * k);
    double res2 = vals.front() + vals.back();
    double temp = 0;
    for (qint32 i = 1; i <= k - 1; i += 2) {
        temp += vals[i];
    }
    res2 += 4 * temp;
    temp = 0;
    for (qint32 i = 2; i <= k - 1; i += 2) {
        temp += vals[i];
    }
    res2 += 2 * temp;
    return res1 * res2;
}

double MainWindow::_adapt_simpson(double left, double right, qint32 start_k, qint32 steps) {
    double ans = _simpson(left, right, start_k);
    start_k <<= 1; // умножаем на 2
    double ans2 = _simpson(left, right, start_k);
    if (steps && qAbs(ans - ans2) > _epsilon_val * 15) {  // оценка по Рунге
        double middle = (left + right) / 2;
        _simpson_gra->addData(middle, _func(middle));
        steps--;
        return _adapt_simpson(left, middle, start_k, steps) + _adapt_simpson(middle, right, start_k, steps);
    }
    return ans2;
}

void MainWindow::_m_c(qint32 n_rolls, qint32 n) {
    QVector<double> vals(n);
    double lenght = _right - _left;
    double height = _up - _down;
    double square = lenght * height;
    for (qint32 epoch = 0; epoch < n; ++epoch){
        qint32 counter = 0;
        for (qint32 i = 0; i < n_rolls; ++i) {
            double x = _left + _gen->generateDouble() * lenght;
            double y = _down + _gen->generateDouble() * height;
            double true_y = _func(x);
            if (true_y >= 0) {
                if (true_y >= y && y >= 0) {
                    _ok_gra->addData(x, y);
                    counter++;
                } else {
                    _not_gra->addData(x, y);
                }
            } else {
                if (true_y <= y && y <= 0) {
                    _ok_gra->addData(x, y);
                    counter--;
                } else {
                    _not_gra->addData(x, y);
                }
            }
        }
        vals[epoch] = square * counter / n_rolls;
    }
    _m_c_result->setText(QString("MC resut: ") + QString::number(std::accumulate(vals.begin(), vals.end(), 0.0)/(double)n, 'g', 10));
    _m_c_dev->setText(QString("MC dev: ") + QString::number(_std_dev(vals), 'g', 10));
}

void MainWindow::_on_click() {
    _ind->set("ВЫЧИСЛЕНИЯ", QColor("red"));
    QString raw_func = _func_row->text();
    _parser->compile(raw_func.toStdString(), *_expression);
    _left = _left_row->text().toDouble();
    _right = _right_row->text().toDouble();
    _left = qMin(_left, _right);
    _up = _up_row->text().toDouble();
    _down = _down_row->text().toDouble();
    _down = qMin(_up, _down);
    _epsilon_val = qPow(10, _epsilon_row->text().toInt());
    _qplot->xAxis->setRange(_left, _right);
    _qplot->yAxis->setRange(_down, _up);
    _simpson_gra->data()->clear();
    _ok_gra->data()->clear();
    _not_gra->data()->clear();
    _func_gra->data()->clear();
//    _qplot->replot();
    _simpson_result->setText(QString("simpson resut: ") + QString::number(_adapt_simpson(_left, _right, 2, _steps_row->text().toInt()), 'g', 10));
    _m_c(_rol_row->text().toInt(), qMax(_epoch_row->text().toInt(), 1));
    qint32 val = _dot_row->text().toInt();
    double step = (_right - _left) /  val;
    double start = _left;
    for (qint32 x = 0; x <= val; ++x){
        _func_gra->addData(start, _func(start));
        start += step;
    }
    _qplot->replot();
    _ind->set("OK", QColor("green"));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _qplot(new QCustomPlot())
    , _ok_gra(_qplot->addGraph())
    , _not_gra(_qplot->addGraph())
    , _func_gra(_qplot->addGraph())
    , _simpson_gra(_qplot->addGraph())
    , _ind(new indicator())
    , _plot_but(new QPushButton("Построить"))
    , _m_c_result(new QLabel())
    , _m_c_dev(new QLabel())
    , _simpson_result(new QLabel())
    , _func_row(new row("функция", "1/(1 - 0.49*(sin(x))^2)"))
    , _left_row(new row("левая граница", "0"))
    , _right_row(new row("правая граница", "1.6"))
    , _up_row(new row("верхняя граница", "2"))
    , _down_row(new row("нижняя граница", "-0.5"))
    , _epsilon_row(new row("точность", "-6"))
    , _dot_row(new row("число точек", "100"))
    , _rol_row(new row("число бросков", "10000"))
    , _epoch_row(new row("число попыток", "10"))
    , _steps_row(new row("число шагов рекурсии", "10"))
    , _gen(new QRandomGenerator())
    , _expression(new exprtk::expression<double>())
    , _parser(new exprtk::parser<double>())
{
    _qplot->setFixedSize(600, 600);
    _ok_gra->setAdaptiveSampling(false);
    _ok_gra->setLineStyle(QCPGraph::lsNone);
    _ok_gra->setScatterStyle(QCPScatterStyle::ssDot);
    _not_gra->setAdaptiveSampling(false);
    _not_gra->setLineStyle(QCPGraph::lsNone);
    _not_gra->setScatterStyle(QCPScatterStyle::ssDot);
    _simpson_gra->setAdaptiveSampling(false);
    _simpson_gra->setLineStyle(QCPGraph::lsNone);
    _simpson_gra->setScatterStyle(QCPScatterStyle::ssDot);
    _ok_gra->setPen(QPen(QColor("green"), 5));
    _not_gra->setPen(QPen(QColor("blue"), 5));
    _simpson_gra->setPen(QPen(QColor("red"), 5));
    _func_gra->setPen(QPen(QColor("black"), 5));
    exprtk::symbol_table<double> table;
    table.add_variable("x", _cur_x);
    table.add_constants();
    _expression->register_symbol_table(table);
    QLocale lo(QLocale::C);
    lo.setNumberOptions(QLocale::RejectGroupSeparator);
    QDoubleValidator* val = new QDoubleValidator(this);
    val->setLocale(lo);
    QHBoxLayout* main = new QHBoxLayout();
    QVBoxLayout* panel = new QVBoxLayout();
    main->addWidget(_qplot);
    panel->addWidget(_ind);
    panel->addWidget(_m_c_result);
    panel->addWidget(_m_c_dev);
    panel->addWidget(_simpson_result);
    panel->addWidget(_func_row);
    panel->addWidget(_left_row);
    _left_row->setValidator(val);
    panel->addWidget(_right_row);
    _right_row->setValidator(val);
    panel->addWidget(_down_row);
    _down_row->setValidator(val);
    panel->addWidget(_up_row);
    _up_row->setValidator(val);
    panel->addWidget(_dot_row);
    _dot_row->setValidator(val);
    panel->addWidget(_rol_row);
    _rol_row->setValidator(new QIntValidator());
    panel->addWidget(_epoch_row);
    _epoch_row->setValidator(new QIntValidator());
    panel->addWidget(_epsilon_row);
    _epsilon_row->setValidator(new QIntValidator());
    panel->addWidget(_steps_row);
    _steps_row->setValidator(new QIntValidator());
    panel->addWidget(_plot_but);
    panel->setSpacing(0);
    panel->setContentsMargins(0, 0, 0, 0);
    main->addLayout(panel);
    main->setSpacing(0);
    main->setContentsMargins(0, 0, 0, 0);
    QWidget *main_wig = new QWidget();
    main_wig->setLayout(main);
    this->setCentralWidget(main_wig);
    connect(_plot_but, &QPushButton::clicked, this, &MainWindow::_on_click);
    this->setFixedSize(900, 600);
}

MainWindow::~MainWindow() {
    delete _gen;
    delete _expression;
    delete _parser;
}
