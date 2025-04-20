#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "TypeInfo.h"
#include "PolynomialDefines.h"
#include <stdbool.h> 

typedef struct {
    void** coefficients;
    int degree;
    TypeInfo* typeInfo;
} Polynomial;

Polynomial* poly_create(TypeInfo*, int, PolynomialError*);
Polynomial* poly_create_with_coeffs(TypeInfo*, int, const void*, PolynomialError*);
void poly_free(Polynomial*);
PolynomialError poly_add(const Polynomial*, const Polynomial*, Polynomial*);
PolynomialError poly_multiply(const Polynomial*, const Polynomial*, Polynomial*);
PolynomialError poly_scalar_multiply(const Polynomial*, const void*, Polynomial*);
PolynomialError poly_evaluate(const Polynomial*, const void*, void*);
PolynomialError poly_compare(const Polynomial*, const Polynomial*);
void poly_print(const Polynomial*);
bool poly_is_equal(const Polynomial* a, const Polynomial* b);

#endif
