#include "control/Stability.hpp"
#include <cmath>

using namespace std;

RouthResult Stability::routhHurwitz(const Polynomial& p) {
    vector<double> a = p.coefficientsDescending(); // [a_n, a_{n-1}, ..., a_0]
    int n = static_cast<int>(a.size()) - 1;

    RouthResult result;
    result.rightHalfPlaneRoots = 0;
    result.stable = true;
    result.marginallyStable = false;
    if(n < 1) return result;

    int rows = n+1;
    int cols = n/2+2;


    // s^n  : a_n, a_{n-2}, a_{n-4}, ...
    // s^n-1: a_{n-1}, a_{n-3}, a_{n-5}, ...
    vector<vector<double>> table(rows, vector<double>(cols, 0.0));
    
    for(int i = 0 ; i <= n;i=i+2){ table[0][i/2]     = a[i]; }
    for(int i = 1 ; i <= n;i=i+2){ table[1][(i-1)/2] = a[i]; }
    
    const double eps = 1e-9;
    bool zeroRow = false;
    // we check row s^n-1
    {
        bool row1AllZero = true;
       for(int c = 0; c < cols; c++) 
            if(fabs(table[1][c]) > eps){
                 row1AllZero = false; break; }
        // if row 1 is full of zeros, the we derive row 0
        if(row1AllZero){
            for(int c = 0; c < cols; c++){
                double exponent = n - 2*c;
                table[1][c] = exponent * table[0][c]; 
            }
        }
    }

    for(int r = 2; r <= n; r++){
        //if first value is zero, we change it with a very small number eps
        if(fabs(table[r - 1][0]) < eps) table[r - 1][0] = eps;

        for(int c = 0; c < cols - 1; c++){
        double a1 = table[r - 2][0], a2 = table[r - 2][c + 1];
        double b1 = table[r - 1][0], b2 = table[r - 1][c + 1];
        table[r][c] = (b1 * a2 - a1 * b2) / b1; //determinant
        }       
        
        bool allZero = true;
        for(int c = 0 ; c< cols ; c++){
            if(fabs(table[r][c]) > eps) {allZero = false; break;}
        }
        //we derive the line above
        if(allZero){
            zeroRow = true;

            int power = n - (r - 1);

            for(int c = 0; c < cols; c++){
                int exponent = power - 2 * c;
                table[r][c] = exponent * table[r - 1][c];
            }   
        }
    }

    int signChanges = 0;
    double prevSign = (table[0][0] >= 0) ? 1.0 : -1.0;
    for(int r = 1; r <= n; r++){
        double sign = (table[r][0] >= 0) ? 1.0 : -1.0;
        if(sign != prevSign) signChanges++;
        prevSign = sign;
    }
    
    result.table = table;
    result.rightHalfPlaneRoots = signChanges;

    result.marginallyStable = zeroRow && (signChanges == 0);
    result.stable = (signChanges == 0) && !zeroRow;
    return result;
}