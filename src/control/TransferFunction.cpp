#include "control/TransferFunction.hpp"

using namespace std;

TransferFunction::TransferFunction(Polynomial numerator, Polynomial denominator) :
    beta(move(numerator)), alpha(move(denominator)){}

ComplexNumber TransferFunction::evaluate(const ComplexNumber& s) const{
    return beta.evaluate(s)/alpha.evaluate(s);
}

vector<ComplexNumber> TransferFunction::poles() const{
    return alpha.roots();
}

vector<ComplexNumber> TransferFunction::zeros() const {
    return beta.roots();
}

const Polynomial& TransferFunction::numerator() const   { return beta;  }
const Polynomial& TransferFunction::denominator() const { return alpha; }

Polynomial TransferFunction::closedLoopCharacteristicPolynomial(double k) const {
    return alpha + beta * k;
}

vector<ComplexNumber> TransferFunction::closedLoopPoles(double k) const {
    return closedLoopCharacteristicPolynomial(k).roots();
}
