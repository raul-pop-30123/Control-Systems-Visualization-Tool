#include "Matrix.hpp"
#include <cstdlib>

using namespace std;

// Raw methods

void Matrix::allocateMemory(){
    if(rows <= 0 || cols <= 0){
        data = nullptr;
        return;
    }

    data = (double**)malloc(rows * sizeof(double*));

    if(data == nullptr){
        throw bad_alloc();
    }
    for(int i = 0; i < rows; i++){
        data[i] = (double*)malloc(cols * sizeof(double));

        if(data[i] == nullptr){
            for(int k = 0; k < i; k++){
                    free(data[k]);
            }
            free(data);
            data = nullptr;
            throw bad_alloc();
        }

        
    }
}

void Matrix::freeMemory(){
    if(data == nullptr){
        return;
    }

    for(int i=0 ; i < rows; i++){
        if(data[i] != nullptr){
            free(data[i]);
        }
    }
    free(data);

    data = nullptr;
    rows = 0;
    cols = 0;
}

void Matrix::resize(int newRows, int newCols, double defaultValue) {
    if (newRows <= 0 || newCols <= 0) {
        freeMemory();
        return;
    }

    if (newRows < rows) {
        for (int i = newRows; i < rows; ++i) {
            if (data != nullptr && data[i] != nullptr) {
                free(data[i]);
            }
        }
    }

    double** tempRows = (double**)realloc(data, newRows * sizeof(double*));
    if (tempRows == nullptr) {
        throw bad_alloc();
    }
    data = tempRows;

    if (newRows > rows) {
        for (int i = rows; i < newRows; ++i) {
            data[i] = (double*)malloc(newCols * sizeof(double));
            if (data[i] == nullptr) throw bad_alloc();
            for (int j = 0; j < newCols; ++j) {
                data[i][j] = defaultValue;
            }
        }
    }

    int existingRowsToResize = (newRows < rows) ? newRows : rows;
    for (int i = 0; i < existingRowsToResize; ++i) {
        double* tempCols = (double*)realloc(data[i], newCols * sizeof(double));
        if (tempCols == nullptr) throw bad_alloc();
        data[i] = tempCols;

        for (int j = cols; j < newCols; ++j) {
            data[i][j] = defaultValue;
        }
    }

    rows = newRows;
    cols = newCols;
}

Matrix::Matrix(int r, int c, double initialValue) : rows(r), cols(c) {
    if (r < 0 || c < 0) {
        throw invalid_argument("Matrix dimensions cannot be negative.");
    }

    allocateMemory();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = initialValue;
        }
    }
}

Matrix::Matrix() : rows(0), cols(0), data(nullptr) {}

Matrix::~Matrix() {freeMemory();}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols){
    allocateMemory();

    for(int i = 0 ; i < rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            data[i][j] = other.data[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& other){
    if (this == &other) return *this;

    freeMemory();
    rows = other.rows;
    cols = other.cols;

    allocateMemory();

    for(int i = 0 ; i< rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            data[i][j] = other.data[i][j];
        }
    }

    return *this;
}

int Matrix::getRows() const { return rows; }

int Matrix::getCols() const { return cols; }

double& Matrix::at(int r, int c){
    if(r < 0 || r >= rows || c < 0 || c >= cols){
        throw out_of_range("Matrix index out of bounds!");
    }
    return data[r][c];
}

const double& Matrix::at(int r, int c) const{
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        throw out_of_range("Matrix index out of bounds.");
    }
    return data[r][c];
}

Matrix Matrix::operator+(const Matrix& other) const{
    if(rows != other.rows || cols != other.cols){
        throw invalid_argument("Matrix dimensions must match");
    }

    Matrix result(rows, cols);
    for(int i = 0 ; i < rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const{
    if(rows != other.rows || cols != other.cols){
        throw invalid_argument("Matrix dimensions must match");
    }

    Matrix result(rows, cols);
    for(int i = 0 ; i < rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const{
    if(cols != other.rows){
        throw invalid_argument("Matrix inner dimensions must match for multiplication.");
    }

    Matrix result(rows, other.cols, 0.0);
    for(int i = 0 ; i < rows ; i++){
        for(int j = 0 ; j < other.cols ; j++){
            for(int k = 0 ; k < cols ; k ++){
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

Matrix Matrix::transpose() const{
    Matrix result(cols,rows);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}

double Matrix::determinant() const{
    if (rows != cols) {
        throw invalid_argument("Determinant is only defined for square matrices.");
    }
    if (rows == 0) return 0.0;          // rank 0
    if (rows == 1) return data[0][0];   // rank 1
    if (rows == 2) {                    // rank 2
        return (data[0][0] * data[1][1]) - (data[0][1] * data[1][0]);
    }
                                        //rank >=3
    double det = 0.0;
    for (int p = 0; p < cols; ++p) {
        Matrix subMatrix(rows - 1, cols - 1);
        for (int i = 1; i < rows; ++i) {
            int subCol = 0;
            for (int j = 0; j < cols; ++j) {
                if (j == p) continue;
                subMatrix.data[i - 1][subCol] = data[i][j];
                subCol++;
            }
        }
        double sign = (p % 2 == 0) ? 1.0 : -1.0;
        det += sign * data[0][p] * subMatrix.determinant();
    }
    return det;
}

ostream& operator<<(ostream& os, const Matrix& m) {
    for (int i = 0; i < m.rows; ++i) {
        os << "[ ";
        for (int j = 0; j < m.cols; ++j) {
            os << m.data[i][j] << " ";
        }
        os << "]\n";
    }
    return os;
}