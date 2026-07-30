#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <stdexcept>

class Matrix{
private:
    int rows;
    int cols;
    double** data;

    void allocateMemory();
    void freeMemory();

public:
    ~Matrix();
    Matrix();
    Matrix(int r, int c, double initialValue = 0.0);
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);

    int getRows() const;
    int getCols() const;

    void resize(int newRows, int newCols, double defaultValue = 0.0);

    double& at(int r, int c);

    const double& at(int r, int c) const;

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;

    Matrix transpose() const;
    double determinant() const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};
#endif