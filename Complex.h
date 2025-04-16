#ifndef COMPLEX_H
#define COMPLEX_H

#include "TypeInfo.h"

typedef struct {
    double real;
    double imag;
} Complex;

extern TypeInfo* COMPLEX_TYPE_INFO;

void complex_add(const void*, const void*, void*);
void complex_multiply(const void*, const void*, void*);
void complex_multiply_scalar(const void*, const void*, void*);
void complex_evaluate(const void*, const void*, void*);
void complex_print(const void*);
TypeInfo* GetComplexTypeInfo();

#endif