#include "Matrix.hpp"
#include <cstdlib>

using namespace std;

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

void Matrix::resize(int newRows, int newCols, double defaultValue){
    if(newRows <=0 || newCols <=0){
        freeMemory();
        return;
    }

    double** tempRows = (double**)realloc(data,newRows * sizeof(double*));
    if(tempRows == nullptr){
        throw bad_alloc();
    }
    data = tempRows;

    if (newRows < rows){
        for (int i = newRows; i < rows; ++i) {
            free(data[i]);
        }
    } else if (newRows > rows){
        // Allocate space for new rows
        for (int i = rows; i < newRows; ++i) {
            data[i] = (double*)malloc(newCols * sizeof(double));
            if(data[i] == nullptr) throw bad_alloc();
            for(int j = 0 ; j < newCols ; j++) data[i][j] = defaultValue;
        }
    }
    // If newCols is greater than cols, then
    // we need to modify the existing rows

    int rowsToResize = newRows < rows ? newRows : rows ; 
    for(int i = 0 ; i < rowsToResize ; i++){
        double* tempCols = (double*)realloc(data[i],newCols * sizeof(double));  //create a new pointer with desired length
        if(tempCols == nullptr) throw bad_alloc();
        data[i] = tempCols; // point towards new vector with required length;

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

Matrix::~Matrix() {freeMemory();}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols){
    allocateMemory();

    for(int i = 0 ; i < rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            data[i][j] = other.data[i][j];
        }
    }
}
