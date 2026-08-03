#ifndef TRANSFER_FUNCTION_HPP
#define TRANSFER_FUNCTION_HPP

#include <vector>
#include "math/Polynomial.hpp"

// H(s) = beta(s) / alpha(s)

class TransferFunction {
private:
    Polynomial beta; // numerator
    Polynomial alpha; // denominator

public:
    TransferFunction(Polynomial numerator, Polynomial denominator);
    ComplexNumber evaluate(const ComplexNumber& s) const;

    std::vector<ComplexNumber> poles() const;
    std::vector<ComplexNumber> zeros() const;

    const Polynomial& numerator() const;
    const Polynomial& denominator() const;

    //  1 + k*Hdes(s) = 0  <=>  alpha(s) + k*beta(s) = 0
    Polynomial closedLoopCharacteristicPolynomial(double k) const;
    std::vector<ComplexNumber> closedLoopPoles(double k) const;
};

#endif
