#include "mainwindow.h"
#include <QDebug>


#define RED _ind->set("вычисления", QColor("red"))
#define GREEN _ind->set("ок", QColor("green"))

void MainWindow::_build()
{
    _ind->set("вычисления", QColor("red"));
    qint32 col = qMax(1, _col_row->text().toInt());
    _mat->build(col, col + 1);
    _ind->set("ок", QColor("green"));
}

void MainWindow::_gauss()
{
    RED;
    QVector<QVector<double>> nums = _mat->data();
    qint32 rows = nums.size(), columns = nums[0].size();
    for (qint32 i = 0; i < rows; ++i) { // rows - 1
        std::sort(nums.begin() + i, nums.end(), [i](QVector<double> a, QVector<double>b){return a[i] > b[i];});
        if (nums[i][i] == 0 && (nums[i][i] != nums[rows - 1][i])) {
            qSwap(nums[i][i], nums[rows - 1][i]);
        }
        for (qint32 j = i + 1; j < columns; ++j) {
            nums[i][j] /= nums[i][i];
        }
        nums[i][i] = 1;
        for (qint32 k = i + 1; k < rows; ++k) {
            for (qint32 j = i + 1; j < columns; ++j) {
                nums[k][j] -= nums[i][j] * nums[k][i];
            }
            nums[k][i] = 0;
        }
    }
    QString ans;
    for (qint32 i = rows - 1; i >=0; --i){
        double temp = std::accumulate(nums[i].begin() + i + 1, nums[i].end() - 1, 0.0);
        double res = (nums[i][columns - 1] - temp);
        ans.push_front(QString::number(res) + " ");
        for (qint32 j = i - 1; j >= 0; --j){
            nums[j][i] *= res;
        }
    }
    _gauss_lab->setText(ans);
    GREEN;
}

void MainWindow::_gd()
{
    RED;
    QVector<QVector<double>> nums = _mat->data();
    Matrix b = Matrix(nums).column_at(nums[0].size() - 1);
    for (auto& i: nums) i.pop_back();
    Matrix a = Matrix(nums);
    qint32 rows = a.size().first;
    Matrix x = b;
    for (qint32 i = 0; i < rows; ++i) {
        x.at(i, 0) = a.at(i, i);
    }
    Matrix r = a * x - b;
    double eps = qPow(10, -_eps_row->text().toDouble());
    qint32 steps = _gd_steps_row->text().toDouble();
    while ((r.transpose()  * r).at(0,0) > eps && steps--) {
        Matrix temp = a * a.transpose() * r;
        double mu = (r.transpose() * temp).at(0,0) / (temp.transpose() * temp).at(0,0);
        x = x - a.transpose() * r * mu;
        r = a * x - b;
    }
    QString ans;
    for (qint32 i = rows - 1; i >=0; --i){
        ans.push_front(QString::number(x.at(i, 0)) + " ");
    }
    _gradient_lab->setText(ans);
    GREEN;
}

void MainWindow::_cgm()
{
    RED;
    QVector<QVector<double>> nums = _mat->data();
    Matrix b = Matrix(nums).column_at(nums[0].size() - 1);
    for (auto& i: nums) i.pop_back();
    Matrix a = Matrix(nums);
    qint32 rows = a.size().first;
    Matrix x = b;
    for (qint32 i = 0; i < rows; ++i) {
        x.at(i, 0) = a.at(i, i);
    }
    double eps = qPow(10, -_cgm_eps_row->text().toDouble());
    qint32 steps = _cgm_steps_row->text().toDouble();
    Matrix r = b - a * x;
    Matrix p = r;
    while ((r.transpose()  * r).at(0,0) > eps && steps--) {
        double alpha = (r.transpose() * r).at(0,0) / (p.transpose() * (a * p)).at(0,0);
        x = x + p * alpha;
        Matrix new_r = r - a * p * alpha;
        double beta = (new_r.transpose() * new_r).at(0, 0) / (r.transpose() * r).at(0, 0);;
        r = new_r;
        p = r + p * beta;
    }
    QString ans;
    for (qint32 i = rows - 1; i >=0; --i){
        ans.push_front(QString::number(x.at(i, 0)) + " ");
    }
    _cgm_lab->setText(ans);
    GREEN;
}

void MainWindow::_reg()
{
    RED;
    QVector<QVector<double>> nums = _mat->data();
    Matrix b = Matrix(nums).column_at(nums[0].size() - 1);
    for (auto& i: nums) i.pop_back();
    Matrix a = Matrix(nums);
    qint32 rows = a.size().first;
    Matrix x = b;
    for (qint32 i = 0; i < rows; ++i) {
        x.at(i, 0) = a.at(i, i);
    }
    Matrix r = a * x - b;
    double eps = qPow(10, -_reg_eps_row->text().toDouble());
    double alpha = qPow(10, -_reg_alpha_row->text().toDouble());
    qint32 steps = _reg_steps_row->text().toDouble();
    while ((r.transpose()  * r).at(0,0) > eps && steps--) {
        Matrix q = a.transpose() * r - x * alpha;
        Matrix temp = a * q;
        double mu = (    (r.transpose() * temp).at(0,0) - alpha * (x.transpose() * q).at(0,0)    )
                / //------------------------------------------------------------------------------
                    (  (temp.transpose() * temp).at(0,0) + alpha * (q.transpose() * q).at(0,0)   );
        x = x - a.transpose() * r * mu;
        r = a * x - b;
    }
    QString ans;
    for (qint32 i = rows - 1; i >=0; --i){
        ans.push_front(QString::number(x.at(i, 0)) + " ");
    }
    _reg_lab->setText(ans);
    GREEN;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _mat(new matrix_widget)
    , _ind(new indicator)
    , _build_but(new QPushButton("задать матрицу"))
    , _gauss_but(new QPushButton("метод Гаусса"))
    , _gradient_but(new QPushButton("метод градиентного спуска"))
    , _cgm_but(new QPushButton("метод сопряженных градиентов"))
    , _reg_but(new QPushButton("регуляризация"))
    , _col_row(new row("переменные", "3"))
    , _eps_row(new row("точность", "3"))
    , _gd_steps_row(new row("число шагов", "50"))
    , _cgm_eps_row(new row("точность", "3"))
    , _cgm_steps_row(new row("число шагов", "50"))
    , _reg_eps_row(new row("точность", "3"))
    , _reg_alpha_row(new row("alpha", "3"))
    , _reg_steps_row(new row("число шагов", "50"))
    , _gauss_lab(new QLabel)
    , _gradient_lab(new QLabel)
    , _cgm_lab(new QLabel)
    , _reg_lab(new QLabel)
{
    QWidget* wig = new QWidget;
    QHBoxLayout* main_lay = new QHBoxLayout;
    QVBoxLayout* panel = new QVBoxLayout;
    _col_row->setValidator(new QIntValidator(1, 100));
    _eps_row->setValidator(new QIntValidator(1, 10));
    _gd_steps_row->setValidator(new QIntValidator(1, 100000));
    _cgm_steps_row->setValidator(new QIntValidator(1, 100000));
    _cgm_eps_row->setValidator(new QIntValidator(1, 10));
    _reg_steps_row->setValidator(new QIntValidator(1, 100000));
    _reg_alpha_row->setValidator(new QIntValidator(1, 10));
    _reg_eps_row->setValidator(new QIntValidator(1, 10));
    main_lay->addWidget(_mat);
    main_lay->addLayout(panel);
    panel->addWidget(_ind);
    panel->addWidget(_col_row);
    panel->addWidget(_build_but);
    _gauss_lab->setWordWrap(true);
    panel->addWidget(_gauss_lab);
    panel->addWidget(_gauss_but);
    panel->addWidget(_eps_row);
    panel->addWidget(_gd_steps_row);
    _gradient_lab->setWordWrap(true);
    panel->addWidget(_gradient_lab);
    panel->addWidget(_gradient_but);
    panel->addWidget(_cgm_eps_row);
    panel->addWidget(_cgm_steps_row);
    _cgm_lab->setWordWrap(true);
    panel->addWidget(_cgm_lab);
    panel->addWidget(_cgm_but);
    panel->addWidget(_reg_eps_row);
    panel->addWidget(_reg_alpha_row);
    panel->addWidget(_reg_steps_row);
    _reg_lab->setWordWrap(true);
    panel->addWidget(_reg_lab);
    panel->addWidget(_reg_but);
    panel->setContentsMargins(10, 10, 10, 10);
    panel->setAlignment(Qt::AlignTop);
    _mat->setFixedSize(500, 500);
    _mat->build(3,4);
    wig->setLayout(main_lay);
    this->setCentralWidget(wig);
    this->setFixedSize(1000, 600);
    connect(_build_but, &QPushButton::clicked, this, &MainWindow::_build);
    connect(_gauss_but, &QPushButton::clicked, this, &MainWindow::_gauss);
    connect(_gradient_but, &QPushButton::clicked, this, &MainWindow::_gd);
    connect(_cgm_but, &QPushButton::clicked, this, &MainWindow::_cgm);
    connect(_reg_but, &QPushButton::clicked, this, &MainWindow::_reg);
}
