#include "control/Stability.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

RouthResult Stability::routhHurwitz(const Polynomial& p) {
    vector<double> a = p.coefficientsDescending();
    
    RouthResult result;
    result.rightHalfPlaneRoots = 0;
    result.stable = false;
    result.marginallyStable = false;

    if (a.empty()) return result; 

    int originRoots = 0;
    while (!a.empty() && fabs(a.back()) < 1e-12) {
        a.pop_back();
        originRoots++;
    }

    int n = static_cast<int>(a.size()) - 1;

    if (n < 1) {
        result.stable = (originRoots == 0);
        result.marginallyStable = (originRoots == 1);
        return result;
    }

    if (a[0] < 0) {
        for (double& coeff : a) coeff = -coeff;
    }

    int rows = n + 1;
    int cols = n / 2 + 2;
    const double eps = 1e-9;

    vector<vector<double>> table(rows, vector<double>(cols, 0.0));
    
    for (int i = 0; i <= n; i += 2) { table[0][i / 2]       = a[i]; }
    for (int i = 1; i <= n; i += 2) { table[1][(i - 1) / 2] = a[i]; }

    bool zeroRow = false;

    for (int r = 2; r <= n; r++) {
        bool prevAllZero = true;
        for (int c = 0; c < cols; c++) {
            if (fabs(table[r - 1][c]) > eps) { 
                prevAllZero = false; 
                break; 
            }
        }

        if (prevAllZero) {
            zeroRow = true;
            int power = n - (r - 2); 
            for (int c = 0; c < cols; c++) {
                int exponent = power - 2 * c;
                table[r - 1][c] = (exponent > 0) ? (exponent * table[r - 2][c]) : 0.0;
            }
        } else if (fabs(table[r - 1][0]) < eps) {
            table[r - 1][0] = eps;
        }

        for (int c = 0; c < cols - 1; c++) {
            double a1 = table[r - 2][0], a2 = table[r - 2][c + 1];
            double b1 = table[r - 1][0], b2 = table[r - 1][c + 1];
            table[r][c] = (b1 * a2 - a1 * b2) / b1;
        } 
    }

    int signChanges = 0;
    double prevSign = (table[0][0] >= 0) ? 1.0 : -1.0;

    for (int r = 1; r <= n; r++) {
        double currentSign = (table[r][0] >= 0) ? 1.0 : -1.0;
        if (currentSign != prevSign) {
            signChanges++;
            prevSign = currentSign;
        }
    }

    result.table = table;
    result.rightHalfPlaneRoots = signChanges;
    
    bool hasBoundaryRoots = zeroRow || (originRoots > 0);
    result.marginallyStable = hasBoundaryRoots && (signChanges == 0) && (originRoots <= 1);
    result.stable = (signChanges == 0) && !hasBoundaryRoots;

    return result;
}