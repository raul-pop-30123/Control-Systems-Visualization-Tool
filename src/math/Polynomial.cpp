#include "math/Polynomial.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

Polynomial::Polynomial() : coeffs{0.0} {}

Polynomial::Polynomial(vector<double> coeffsAscending) : coeffs(move(coeffsAscending)) {
    if(coeffs.empty()) coeffs.push_back(0.0);
    trim();
}