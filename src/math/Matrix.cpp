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