#ifndef HELPER_H
#define HELPER_H

#include <numeric>
#include <algorithm>
#include <QVector>

class Helper {
private:
    using dobVec = QVector<double>;
    using dobVec2d = QVector<dobVec>;
    using point  = QPair<double,double>;

    static void write(dobVec2d &matrix, const dobVec &in, qint32  row, qint32  col) {
        qint32  size = in.size();
        for (qint32  i = 0; i < size; ++i) {
            matrix[row][col + i] = in[i];
        }
    }

    static dobVec inverse(const dobVec &vec) {
        dobVec res = vec;
        qint32  size = vec.size();
        for (qint32  i = 0; i < size; ++i) {
            res[i] = -vec[i];
        }
        return res;
    }

    static dobVec derZ(double x) {
        return {x * x * x, x * x, x, 1};
    }

    static dobVec derF(double x) {
        return {3 * x * x, 2 * x, 1, 0};
    }

    static dobVec derS(double x) {
        return {6 * x, 2, 0, 0};
    }

    static dobVec2d unpack(const dobVec &nums, qint32  count){
        qint32  size = nums.size();
        qint32  resSize = size / count;
        dobVec2d res (resSize);
        for (qint32  i = 0; i < resSize; ++i) {
            qint32  numsI = i << 2;
            dobVec temp {
                nums[numsI],
                nums[numsI + 1],
                nums[numsI + 2],
                nums[numsI + 3]
            };
            res[i] = temp;
        }
        return res;
    }

    static dobVec gauss(dobVec2d nums, dobVec nums2)
    {
        int rows = nums.size();
        for (int i = 0; i < rows; ++i) {
            nums[i].push_back(nums2[i]);
        }
        int columns = nums[0].size();
        for (int i = 0; i < rows; ++i) { // rows - 1
            std::sort(nums.begin() + i, nums.end(), [i](dobVec a, dobVec b){return qAbs(a[i]) > qAbs(b[i]);});
            if (nums[i][i] == 0 && (nums[i][i] != nums[rows - 1][i])) {
                std::swap(nums[i][i], nums[rows - 1][i]);
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
        dobVec ans;
        for (int i = rows - 1; i >=0; --i){
            double temp;
            temp = std::accumulate(nums[i].begin() + i + 1, nums[i].end() - 1, 0.0);
            double res = (nums[i][columns - 1] - temp);
            ans.push_back(res);
            for (int j = i - 1; j >= 0; --j){
                nums[j][i] *= res;
            }
        }
        std::reverse(ans.begin(), ans.end());
        return ans;
    }

public:
    static dobVec2d getSplines(const QVector<point> &points) {
        qint32  nPoints =  (qint32 )points.size();
        qint32  nSplines = nPoints - 1;
        qint32  colSize = 4 * nSplines;

        dobVec2d aMatrix(colSize, dobVec(colSize, 0));
        dobVec bVec(colSize, 0);

        for (qint32  i = 0; i < nSplines; ++i) {
            qint32  row = i << 1;
            qint32  col = i << 2;
            bVec[row] = points[i].second;
            Helper::write(aMatrix, Helper::derZ(points[i].first), row, col);
            bVec[row + 1] = points[i + 1].second;
            Helper::write(aMatrix,Helper::derZ(points[i + 1].first), row + 1, col);
        }

        qint32  half = colSize >> 1;
        for (qint32  i = 0; i < nSplines - 1; ++i) {
            qint32  row = half + (i << 1);
            qint32  col = i << 2;
            auto t = Helper::derF(points[i + 1].first);
            Helper::write(aMatrix, t, row, col);
            t = Helper::inverse(Helper::derF(points[i + 1].first));
            Helper::write(aMatrix, t, row, col + 4);
            t = Helper::derS(points[i + 1].first);
            Helper::write(aMatrix, t, row + 1, col);
            t = Helper::inverse(Helper::derS(points[i + 1].first));
            Helper::write(aMatrix, t, row + 1, col + 4);
        }

        {
            qint32  row = colSize - 2;
            auto t = Helper::derS(points[0].first);
            Helper::write(aMatrix, t, row, 0);
            t = Helper::derS(points[nPoints - 1].first);
            Helper::write(aMatrix, t, row + 1, colSize - 4);
        }

        auto res = gauss(aMatrix, bVec);
        return Helper::unpack(res, 4);
    }

    static double splineValue(const dobVec &vec, double x) {
        return (
                x*x*x * vec[0] +
                x*x * vec[1] +
                x * vec[2] +
                vec[3]
        );
    }
};

#endif // HELPER_H
