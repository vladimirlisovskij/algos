//
// Created by vladimir on 27.03.2021.
//

#ifndef NEWTON_FUNCTIONMATRIX_H
#define NEWTON_FUNCTIONMATRIX_H

#include <memory>
#include <utility>
#include <vector>
#include "exprtk.hpp"

using namespace std;

class FunctionMatrix {
private:
    vector<vector<string>> _funMat;
    vector<double> _bias;
    int _rows;
    exprtk::expression<double> _expression;
    exprtk::symbol_table<double> _table;
    exprtk::parser<double> _parser;
    double _curX = 0;

public:
    explicit FunctionMatrix(const vector<vector<string>> &funMat) {
        _funMat = funMat;
        _table.add_variable("x", _curX);
        _table.add_constants();
        _expression.register_symbol_table(_table);
        _rows = _funMat.size();
        _bias = vector<double> (_rows, 0);
    }

    vector<double> getResult (const vector<double> &nums) {
        vector<double> result(_rows, 0);
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _rows; ++j) {
                _parser.compile(_funMat[i][j], _expression);
                _curX = nums[j];
                result[i] += _expression.value();
            }
            result[i] += _bias[i];
        }
        return result;
    }

    void setBias(vector<double> bias) {
        _bias = std::move(bias);
    }
};


#endif //NEWTON_FUNCTIONMATRIX_H
