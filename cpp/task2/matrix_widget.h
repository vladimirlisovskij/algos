#ifndef MATRIX_WIDGET_H
#define MATRIX_WIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QDoubleValidator>
#include <QRandomGenerator>
#include <QDebug>
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>

class matrix_widget : public QWidget
{
    Q_OBJECT
private:
    qint32 _row, _col;
    QGridLayout *_lay;
    QRandomGenerator *_gen;
    QTableWidget *_table;
    QDoubleValidator *_val;

public:
    explicit matrix_widget(QWidget *parent = nullptr)
        : QWidget(parent)
        , _row(0)
        , _col(0)
        , _gen(new QRandomGenerator)
        , _table(new QTableWidget(this))
        , _val(new QDoubleValidator(this))
    {
        QLocale lo(QLocale::C);
        lo.setNumberOptions(QLocale::RejectGroupSeparator);
        _val->setLocale(lo);
        _table->verticalHeader()->setDefaultSectionSize(50);
        _table->verticalHeader()->setVisible(false);
        _table->horizontalHeader()->setDefaultSectionSize(50);
        _table->horizontalHeader()->setVisible(false);
    }

    void resizeEvent(QResizeEvent* event){
        _table->setFixedHeight(this->height());
        _table->setFixedWidth(this->width());
    }

    qint32 rows() {
        return _row;
    }

    qint32 columns() {
        return _col;
    }

    ~matrix_widget() {
        delete _gen;
    }

    void build(qint32 rows, qint32 columns){
        _row = rows;
        _col = columns;
        _table->setColumnCount(columns);
        _table->setRowCount(rows);
        for (qint32 i = 0; i < _row; ++i) {
            for (qint32 j = 0; j < _col; ++j) {
                QLineEdit* line = new QLineEdit(QString::number(0));
                line->setValidator(_val);
                _table->setCellWidget(i, j, line);
            }
        }
    }

    QVector<QVector<double>> data() {
        QVector<QVector<double>> res (_row, QVector<double>(_col));
        for (int i = 0; i < _row; ++i) {
            for (int j = 0; j < _col; ++j) {
                res[i][j] = ((QLineEdit*)_table->cellWidget(i, j))->text().toDouble();
            }
        }
        return res;
    }
};

#endif // MATRIX_WIDGET_H
