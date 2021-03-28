#include <iostream>
#include <vector>
#include <numeric>

#include "FunctionMatrix.h"

using namespace std;

vector<vector<double>> calcJacobi(FunctionMatrix &functionMatrix, const vector<double> &x, double eps) {
    int size = x.size();
    vector<vector<double>> result = vector<vector<double>>(size, vector<double>(size, 0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            vector<double> tempVec = x;
            double right = functionMatrix.getResult(tempVec)[i];
            tempVec[j] += eps;
            double left = functionMatrix.getResult(tempVec)[i];
            result[i][j] = (left - right) / eps;
        }
    }
    return result;
}

vector<double> gauss(vector<vector<double>> nums)
{
    int rows = nums.size(), columns = nums[0].size();
    for (int i = 0; i < rows; ++i) { // rows - 1
        std::sort(nums.begin() + i, nums.end(), [i](vector<double> a, vector<double>b){return a[i] > b[i];});
        if (nums[i][i] == 0 && (nums[i][i] != nums[rows - 1][i])) {
            swap(nums[i][i], nums[rows - 1][i]);
        }
        for (int j = i + 1; j < columns; ++j) {
            nums[i][j] /= nums[i][i];
        }
        nums[i][i] = 1;
        for (int k = i + 1; k < rows; ++k) {
            for (int j = i + 1; j < columns; ++j) {
                nums[k][j] -= nums[i][j] * nums[k][i];
            }
            nums[k][i] = 0;
        }
    }
    vector<double> ans;
    for (int i = rows - 1; i >=0; --i){
        double temp = std::accumulate(nums[i].begin() + i + 1, nums[i].end() - 1, 0.0);
        double res = (nums[i][columns - 1] - temp);
        ans.push_back(res);
        for (int j = i - 1; j >= 0; --j){
            nums[j][i] *= res;
        }
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

int main() {
    vector<vector<string>> matrix {
//            {"cos(x+0.5)", "x"},
//            { "2 * x", "sin(x)"}
            {"1*x", "2*x", "3*x", "4*x", "5*x", "6*x"},
            {"6*x", "1*x", "2*x", "3*x", "4*x", "5*x"},
            {"5*x", "6*x", "1*x", "2*x", "3*x", "4*x"},
            {"4*x", "5*x", "6*x", "1*x", "2*x", "3*x"},
            {"3*x", "4*x", "5*x", "6*x", "1*x", "2*x"},
            {"2*x", "3*x", "4*x", "5*x", "6*x", "1*x"},
    };
    vector<double> bias {
//        -0.8,
//        -1.6
        -10,
        -20,
        -30,
        -40,
        -50,
        -60
    };
    FunctionMatrix mat (matrix);
    mat.setBias(bias);
    vector<double> result(bias.size());
    int size = matrix.size();
    cout << "Введите начальные приближения\n";
    for (int i = 0; i < size; ++i) {
        printf("x%i: ", i+1);
        cin >> result[i];
    }
    for (int step = 0; step < 30; ++step) {
        auto jacobian = calcJacobi(mat, result, 0.001);
        auto functionResVec = mat.getResult(result);
        for (int i = 0; i < size; ++i) jacobian[i].push_back(functionResVec[i]);
        auto delta = gauss(jacobian);
        for (int i = 0; i < size; ++i) result[i] -= delta[i];
    }
    cout << "Вычисленные значения\n";
    for (int i = 0; i < size; ++i) {
        printf("x%i: %lf\n", i+1, result[i]);
    }
    return 0;
}
