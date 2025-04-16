#include "Complex.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

TypeInfo* COMPLEX_TYPE_INFO = NULL;

void complex_add(const void* a, const void* b, void* result) {
    const Complex* ca = a;
    const Complex* cb = b;
    Complex* cr = result;
    cr->real = ca->real + cb->real;
    cr->imag = ca->imag + cb->imag;
}

void complex_multiply(const void* a, const void* b, void* result) {
    const Complex* ca = a;
    const Complex* cb = b;
    Complex* cr = result;
    cr->real = ca->real * cb->real - ca->imag * cb->imag;
    cr->imag = ca->real * cb->imag + ca->imag * cb->real;
}

void complex_multiply_scalar(const void* a, const void* scalar, void* result) {
    complex_multiply(a, scalar, result);
}

void complex_evaluate(const void* coeff, const void* x_power, void* result) {
    complex_multiply(coeff, x_power, result);
}

void complex_print(const void* data) {
    const Complex* c = data;
    if (c->imag == 0.0) {
        printf("%.2f", c->real);
    } else if (c->real == 0.0) {
        printf("%.2fi", c->imag); 
    } else {
        printf("(%.2f %+.2fi)", c->real, c->imag); 
    }
}

TypeInfo* GetComplexTypeInfo() {
    if (!COMPLEX_TYPE_INFO) {
        COMPLEX_TYPE_INFO = malloc(sizeof(TypeInfo));
        COMPLEX_TYPE_INFO->size = sizeof(Complex);
        COMPLEX_TYPE_INFO->add = complex_add;
        COMPLEX_TYPE_INFO->multiply = complex_multiply;
        COMPLEX_TYPE_INFO->multiplyScalar = complex_multiply_scalar;
        COMPLEX_TYPE_INFO->evaluate = complex_evaluate;
        COMPLEX_TYPE_INFO->print = complex_print;
    }
    return COMPLEX_TYPE_INFO;
}