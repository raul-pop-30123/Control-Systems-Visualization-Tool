#include "ComplexNumber.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

ComplexNumber::ComplexNumber() : real(0.0), imaginary(0.0) {}
ComplexNumber::ComplexNumber(double r, double i) : real(r), imaginary(i) {}

double ComplexNumber::magnitude() const {
    return sqrt(real * real + imaginary * imaginary);
}

double ComplexNumber::phase() const {
    return atan2(imaginary, real); 
}

ComplexNumber ComplexNumber::conjugate() const {
    return ComplexNumber(real, -imaginary);
}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
    return ComplexNumber(
        real + other.real,
        imaginary + other.imaginary
    );
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
    return ComplexNumber(
        real - other.real,
        imaginary - other.imaginary
    );
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) const {
    return ComplexNumber(
        real * other.real - imaginary * other.imaginary, 
        imaginary * other.real + real * other.imaginary
    );
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber& other) const {
    double denum = other.real * other.real + other.imaginary * other.imaginary;
    if (denum < 1e-12) {
        throw runtime_error("Division by zero not allowed!");
    }
    return ComplexNumber(
        (real * other.real + imaginary * other.imaginary) / denum,
        (imaginary * other.real - real * other.imaginary) / denum
    );
}

ComplexNumber& ComplexNumber::operator+=(const ComplexNumber& other) {
    real += other.real;
    imaginary += other.imaginary;
    return *this;
}

ComplexNumber& ComplexNumber::operator-=(const ComplexNumber& other) {
    real -= other.real;
    imaginary -= other.imaginary;
    return *this;
}

ostream& operator<<(ostream& os, const ComplexNumber& c) {
    os << c.real;
    if (c.imaginary >= 0) {
        os << " + " << c.imaginary << "j";
    } else {
        os << " - " << -c.imaginary << "j";
    }
    return os;
}