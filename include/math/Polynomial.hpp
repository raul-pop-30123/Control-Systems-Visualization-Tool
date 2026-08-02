#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <vector>
#include "ComplexNumber.hpp"

// Real-coefficient polynomial: coeffs[i] is the coefficient of s^i
// s^2 + 3s + 5 is Polynomial({5, 3, 1}).

class Polynomial{
private:
    using Coefficients = std::vector<double>;
    using RootsList   = std::vector<ComplexNumber>;

    Coefficients coeffs;
    void trim(); // drop trailing (highest-degree) near-zero coefficients
public:
    Polynomial();
    explicit Polynomial(Coefficients coeffsAscending);

    int degree() const;
    double operator[](int i) const; // coefficient of s^i, 0 if out of range
    const Coefficients& coefficients() const;

    // Coefficients in descending order [a_n, ..., a_1, a_0] -- the order
    // Routh-Hurwitz is conventionally written in.
    Coefficients coefficientsDescending() const;
    ComplexNumber evaluate(const ComplexNumber& s) const;
    
    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(double scalar) const;
    Polynomial operator*(const Polynomial& other) const;

    // Durand-Kerner method: finds all n roots simultaneously by iterating
    //   z_i <- z_i - p(z_i) / ( a_n * prod_{j != i} (z_i - z_j) )
    // starting from n points spread around a circle. Simple to reason
    // about (no linear algebra needed) and converges reliably in practice.
    RootsList roots(int maxIterations = 200, double tol = 1e-10) const;
};

#endif
