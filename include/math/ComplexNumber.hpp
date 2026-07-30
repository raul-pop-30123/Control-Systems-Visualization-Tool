#ifndef COMPLEX_NUMBER_HPP
#define COMPLEX_NUMBER_HPP

#include <iostream>

class ComplexNumber{
private:
    double real;
    double imaginary;

public:
    ComplexNumber();
    ComplexNumber(double r, double i=0.0);

    double magnitude() const;
    double phase() const;       //Radian
    ComplexNumber conjugate() const;

    ComplexNumber operator+(const ComplexNumber& other) const;
    ComplexNumber operator-(const ComplexNumber& other) const;
    ComplexNumber operator*(const ComplexNumber& other) const;
    ComplexNumber operator/(const ComplexNumber& other) const;

    ComplexNumber& operator+=(const ComplexNumber& other);
    ComplexNumber& operator-=(const ComplexNumber& other);

friend std::ostream& operator<<(std::ostream& os, const ComplexNumber& c);

};

#endif