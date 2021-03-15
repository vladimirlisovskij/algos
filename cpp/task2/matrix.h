#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>
#include <QtMath>
#include <QPair>

class Matrix
{
private:
    qint32  _rows, _columns;
    QVector<QVector<double>> _matrix;

    double _scalar(const QVector<double>& a, const QVector<double>& b){
        double res = 0;
        qint32  size = a.size();
        for (qint32  i = 0; i < size; ++i) {
            res += a[i] * b[i];
        }
        return res;
    }

public:
    Matrix(QVector<QVector<double>> nums)
        : _rows(nums.size())
        , _columns(nums[0].size())
        , _matrix(nums)
    {}

    QPair<qint32 ,qint32 > size() const {
        return {_rows, _columns};
    }

    QVector<QVector<double>>& to_QVector() {
        return _matrix;
    }

    Matrix row_at(qint32 row) const {
        return Matrix({_matrix[row]});
    }

    Matrix column_at(qint32  column) const {
        QVector<QVector<double>> res(_rows);
        for (qint32  i = 0; i < _rows; ++i){
            res[i] = {_matrix[i][column]};
        }
        return Matrix(res);
    }

    double& at(qint32  row, qint32  column){
        return _matrix[row][column];
    }

    double at (qint32  row, qint32  column) const {
        return _matrix[row][column];
    }

    Matrix transpose() const {
        QVector<QVector<double>> res (_columns, QVector<double>(_rows));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[j][i] = _matrix[i][j];
            }
        }
        return Matrix(res);
    }

    Matrix operator+ (double num) {
        QVector<QVector<double>> res(_rows, QVector<double>(_columns, 0));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[i][j] = num + _matrix[i][j];
            }
        }
        return Matrix(res);
    }

    Matrix operator- (double num) {
        QVector<QVector<double>> res(_rows, QVector<double>(_columns, 0));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[i][j] = _matrix[i][j] - num;
            }
        }
        return Matrix(res);
    }

    Matrix operator* (double num) {
        QVector<QVector<double>> res(_rows, QVector<double>(_columns, 0));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[i][j] = _matrix[i][j] * num;
            }
        }
        return Matrix(res);
    }

    Matrix operator/ (double num) {
        QVector<QVector<double>> res(_rows, QVector<double>(_columns, 0));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[i][j] = _matrix[i][j] / num;
            }
        }
        return Matrix(res);
    }

    Matrix operator+ (const Matrix& num) {
        QVector<QVector<double>> res(_rows, QVector<double>(_columns, 0));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[i][j] = _matrix[i][j] + num.at(i, j);
            }
        }
        return Matrix(res);
    }

    Matrix operator+= (const Matrix& num) {
        return *this + num;
    }

    Matrix operator- (const Matrix& num) {
        QVector<QVector<double>> res(_rows, QVector<double>(_columns, 0));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[i][j] = _matrix[i][j] - num.at(i, j);
            }
        }
        return Matrix(res);
    }

    void operator-= (const Matrix& num) {
        _matrix = (*this - num).to_QVector();
    }

    Matrix operator* (const Matrix& num) {
        QPair<qint32 ,qint32 > s1 = this->size();
        QPair<qint32 ,qint32 > s2 = num.size();
        QVector<QVector<double>> res (s1.first, QVector<double>(s2.second));
        for (qint32  i = 0; i < s1.first; ++i) {
            for (qint32  j = 0; j < s2.second; ++j) {
                res[i][j] = _scalar(this->row_at(i).to_QVector()[0], num.column_at(j).transpose().to_QVector()[0]);
            }
        }
        return Matrix(res);
    }

    Matrix operator/ (const Matrix& num) {
        QVector<QVector<double>> res(_rows, QVector<double>(_columns, 0));
        for (qint32  i = 0; i < _rows; ++i) {
            for (qint32  j = 0; j < _columns; ++j) {
                res[i][j] = _matrix[i][j] / num.at(i, j);
            }
        }
        return Matrix(res);
    }

    static Matrix mult(const Matrix& a, const Matrix& b) {
        QPair<qint32 ,qint32 > size = a.size();
        QVector<QVector<double>> res(size.first, QVector<double>(size.second, 0));
        for (qint32  i = 0; i < size.first; ++i) {
            for (qint32  j = 0; j < size.second; ++j) {
                res[i][j] = a.at(i, j) / b.at(i, j);
            }
        }
        return Matrix(res);
    }
};


#endif // MATRIX_H
