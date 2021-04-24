#ifndef FOURIER_FUNCTIONHELPER_H
#define FOURIER_FUNCTIONHELPER_H

#include <cmath>
#include <memory>

#include "exprtk.hpp"

using namespace std;

class Fourier {
private:
    class FourierHelper{
    private:
        exprtk::expression<double> _expression;
        double _x = 0;
        double _period;
        double _n  = 0;

    public:
        explicit FourierHelper(const string &expr, double period) {
            exprtk::parser<double> parser;
            exprtk::symbol_table<double> table;
            table.add_variable("x", _x);
            table.add_constants();
            _expression.register_symbol_table(table);
            parser.compile(expr, _expression);
            _period = period;
        }

        void setN(double n) {
            _n = n;
        }

        double value(double param) {
            _x = param;
            double res = _expression.value();
            return res;
        }

        double valueCos(double param) {
            return value(param) * cos(_n * M_PI * param / _period);
        }

        double valueSin(double param) {
            return value(param) * sin(_n * M_PI * param / _period);
        }
    };

    double _simpson(double left, double right, int k, double (FourierHelper::*func)(double)) {
        vector<double> vals(k + 1);
        double step = (right - left) / k;
        double cur_step = left;
        for (int i = 0; i <= k; ++i) {
            vals[i] = (*_func.*func)(cur_step);
            cur_step += step;
        }
        double res1 = (right - left) / (3 * k);
        double res2 = vals.front() + vals.back();
        double temp = 0;
        for (int i = 1; i <= k - 1; i += 2) {
            temp += vals[i];
        }
        res2 += 4 * temp;
        temp = 0;
        for (int i = 2; i <= k - 1; i += 2) {
            temp += vals[i];
        }
        res2 += 2 * temp;
        return res1 * res2;
    }

    double _adapt_simpson(double left, double right, int start_k, int steps, double (FourierHelper::*func)(double)) {
        double ans = _simpson(left, right, start_k, func);
        start_k *= 2;
        double ans2 = _simpson(left, right, start_k, func);
        if (steps && abs(ans - ans2) > 0.0001 * 15) {  // оценка по Рунге
            double middle = (left + right) / 2;
            steps--;
            return _adapt_simpson(left, middle, start_k, steps, func) + _adapt_simpson(middle, right, start_k, steps, func);
        }
        return ans2;
    }

    unique_ptr<FourierHelper> _func;
    double _period;
    double _a0Coef;
    vector<double> _aCoefs;
    vector<double> _bCoefs;

public:
    explicit Fourier(const string &function, double left, double right, int nodes, int steps)
    {
        _period = (right - left) / 2.0;
        _func = unique_ptr<FourierHelper>(new FourierHelper(function, _period));

        _a0Coef = _simpson(left, right, steps, &FourierHelper::value) / _period;

        _aCoefs = vector<double> (nodes + 1);
        _bCoefs = vector<double> (nodes + 1);
        for (int i = 1; i <= nodes; ++i) {
            _func->setN(i);
            _aCoefs[i] = _simpson(left, right, steps, &FourierHelper::valueCos) / _period;
            _bCoefs[i] = _simpson(left, right, steps, &FourierHelper::valueSin) / _period;
        }
    }

    double value(double param) {
        double res = _a0Coef / 2.0;
        int size = _aCoefs.size();
        for (int i = 1; i < size; ++i) {
            double  temp1 = _aCoefs[i] * cos ( i * M_PI * param / _period);
            res += temp1;
            double temp2 = _bCoefs[i] * sin ( i * M_PI * param / _period);
            res += temp2;
        }
        return res;
    }

    double rawVal(double param) {
        return _func->value(param);
    }
};


#endif //FOURIER_FUNCTIONHELPER_H
