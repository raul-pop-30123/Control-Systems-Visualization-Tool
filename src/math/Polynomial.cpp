#include "math/Polynomial.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

Polynomial::Polynomial() : coeffs{0.0} {}

Polynomial::Polynomial(vector<double> coeffsAscending) : coeffs(move(coeffsAscending)) {
    if(coeffs.empty()) coeffs.push_back(0.0);
    trim();
}

void Polynomial::trim() {           // trims of unecesary coefficients. 0 + 2s + 0s^2 = 2s 
    while(coeffs.size() > 1 && fabs(coeffs.back()) < 1e-14){
        coeffs.pop_back();
    }       
}

int Polynomial::degree() const {
    return static_cast<int>(coeffs.size()) - 1;         // if there are n+1 terms, it returns n
}

double Polynomial::operator[](int i) const {            //returns polynom[i]
    if(i < 0 || i >= static_cast<int>(coeffs.size())) return 0.0;
    return coeffs[i];
}

const vector<double>& Polynomial::coefficients() const {
    return coeffs;
}

vector<double> Polynomial::coefficientsDescending() const {
    return vector<double>(coeffs.rbegin(), coeffs.rend());
}


// s^3 + 2s^2 + 6s + 3 = s^2 * ( s + 2) + 6s + 3 = 
// s * (s *(s + 2) + 6 ) + 3    =>  s * last_result + Coefficient_descending_order
ComplexNumber Polynomial::evaluate(const ComplexNumber& s) const {
    ComplexNumber result(0.0, 0.0);
    for(int i = degree(); i >= 0; i--){
        result = result * s + ComplexNumber(coeffs[i], 0.0);
    }
    return result;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
    size_t n = max(coeffs.size(), other.coeffs.size());
    vector<double> result(n, 0.0);
    for(size_t i = 0; i < coeffs.size(); i++) result[i] += coeffs[i];
    for(size_t i = 0; i < other.coeffs.size(); i++) result[i] += other.coeffs[i];
    return Polynomial(result);
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
    size_t n = max(coeffs.size(), other.coeffs.size());
    vector<double> result(n, 0.0);
    for(size_t i = 0; i < coeffs.size(); i++) result[i] += coeffs[i];
    for(size_t i = 0; i < other.coeffs.size(); i++) result[i] -= other.coeffs[i];
    return Polynomial(result);
}

Polynomial Polynomial::operator*(double scalar) const {
    vector<double> result = coeffs;
    for(double& c : result) c *= scalar;
    return Polynomial(result);
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
    vector<double> result(coeffs.size() + other.coeffs.size() - 1, 0.0);

    for(size_t i = 0; i < coeffs.size(); i++){
        for(size_t j = 0; j < other.coeffs.size(); j++){
            result[i + j] += coeffs[i] * other.coeffs[j];
    }
}
return Polynomial(result);
}


