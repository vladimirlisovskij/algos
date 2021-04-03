#ifndef LAGRANGE_H
#define LAGRANGE_H

#include <QVector>


class Lagrange{
private:
    using dobVec = QVector<double>;

    class LagrangeHelper{
    private:
        dobVec _x;
        dobVec _bias;
        qint32  _size;

    public:
        LagrangeHelper(qint32 index, const dobVec& values) {
            _size = values.size();
            _x.reserve(_size - 1);
            _bias.reserve(_size - 1);
            double xj = values[index];
            for (qint32  i = 0; i < _size; ++i) {
                if (i == index) continue;
                double temp = (xj - values[i]);
                _x.push_back(temp);
                _bias.push_back(values[i]/temp);
            }
            _size--;
        }

        double getValue(double x) const {
            double res = 1;
            for (qint32  i = 0; i < _size; ++i) {
                res *= x/_x[i] - _bias[i];
            }
            return res;
        }

    };

    using lagVec = QVector<LagrangeHelper>;

    lagVec _lags;
    dobVec _y;
    qint32  _size;

public:
    Lagrange(const dobVec &x, const dobVec &y){
        _y = y;
        _size = y.size();
        _lags.reserve(_size);
        for (qint32  i = 0; i < _size; ++i) {
            _lags.push_back(LagrangeHelper(i, x));
        }
    }

    double getValue(double x) const {
        double res = 0;
        for (qint32  i = 0; i < _size; ++i) {
            res += _y[i] * _lags[i].getValue(x);
        }
        return res;
    }

};

#endif // LAGRANGE_H
