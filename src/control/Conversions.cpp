#include "control/Conversions.hpp"
#include <stdexcept>

using namespace std;

TransferFunction Conversions::toTransferFunction(const StateSpace& ss) {
    int n = ss.order();
    vector<Matrix> M; // M[i] for i = 0..n-1, from Faddeev-LeVerrier
    Polynomial alpha = ss.characteristicPolynomial(&M); // denominator, degree n

    const Matrix& B = ss.getB();
    const Matrix& C = ss.getC();
    double d = ss.getD().at(0, 0);

    // N0(s) = sum_{i=0}^{n-1} (C * M_i * B) * s^(n-1-i)
    vector<double> n0Ascending(n, 0.0); // degrees 0..n-1
    for(int i = 0; i < n; i++){
        Matrix CMB = C * M[i] * B; // 1x1
        n0Ascending[n - 1 - i] = CMB.at(0, 0);
    }
    Polynomial N0(n0Ascending);

    // H(s) = [N0(s) + d*alpha(s)] / alpha(s)
    Polynomial numerator = N0 + alpha * d;
    return TransferFunction(numerator, alpha);
}

StateSpace Conversions::toStateSpace(const TransferFunction& tf) {
    Polynomial alphaRaw = tf.denominator();
    Polynomial betaRaw = tf.numerator();
    int n = alphaRaw.degree();
    int m = betaRaw.degree();

    if(n < 1) throw invalid_argument("Denominator must have degree >= 1");
    if(m > n) throw invalid_argument("Improper transfer function (numerator degree > denominator degree) is not supported by FCC");

    // Normalize so the denominator is monic (alpha[n] == 1); H(s) is
    // unchanged by dividing both polynomials by the same constant.
    double lead = alphaRaw[n];
    Polynomial alpha = alphaRaw * (1.0 / lead);
    Polynomial beta = betaRaw * (1.0 / lead);

    // If numerator and denominator have equal degree, H(s) is proper but
    // not strictly proper: do one step of polynomial division to pull out
    // the direct-transmission term d, per H(s) = d + (strictly proper part).
    double d = 0.0;
    Polynomial remainder = beta;
    if(m == n){
        d = beta[n]; // alpha is monic, so this is the whole division step
        remainder = beta + alpha * (-d);
    }

    Matrix A(n, n, 0.0), B(n, 1, 0.0), C(1, n, 0.0), D(1, 1, 0.0);
    for(int col = 0; col < n; col++){
        A.at(0, col) = -alpha[n - 1 - col];
        C.at(0, col) = remainder[n - 1 - col];
    }
    for(int row = 1; row < n; row++){
        A.at(row, row - 1) = 1.0;
    }
    B.at(0, 0) = 1.0;
    D.at(0, 0) = d;

    return StateSpace(A, B, C, D);
}