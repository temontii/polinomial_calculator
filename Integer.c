#include "Integer.h"
#include <stdio.h>
#include <stdlib.h>

TypeInfo* INT_TYPE_INFO = NULL;

void int_add(const void* a, const void* b, void* result) {
    *((int*)result) = *((const int*)a) + *((const int*)b);
}

void int_multiply(const void* a, const void* b, void* result) {
    *((int*)result) = *((const int*)a) * *((const int*)b);
}

void int_multiply_scalar(const void* a, const void* scalar, void* result) {
    *((int*)result) = *((const int*)a) * *((const int*)scalar);
}

void int_evaluate(const void* coeff, const void* x, void* result) {
    *((int*)result) = *((const int*)coeff) * *((const int*)x);
}

void int_print(const void* data) {
    printf("%d", *((const int*)data));
}

TypeInfo* GetIntTypeInfo() {
    if (!INT_TYPE_INFO) {
        INT_TYPE_INFO = malloc(sizeof(TypeInfo));
        INT_TYPE_INFO->size = sizeof(int);
        INT_TYPE_INFO->add = int_add;
        INT_TYPE_INFO->multiply = int_multiply;
        INT_TYPE_INFO->multiplyScalar = int_multiply_scalar;
        INT_TYPE_INFO->evaluate = int_evaluate;
        INT_TYPE_INFO->print = int_print;
    }
    return INT_TYPE_INFO;
}