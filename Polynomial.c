#include "Polynomial.h"
#include "Integer.h"
#include "Complex.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h> 

Polynomial* poly_create(TypeInfo* typeInfo, int degree, PolynomialError* err) {
    if (!typeInfo) {
        if (err) *err = POLYNOMIAL_NULL_PTR;
        return NULL;
    }
    
    if (degree < 0) {
        if (err) *err = POLYNOMIAL_INVALID_DEGREE;
        return NULL;
    }
    
    Polynomial* poly = malloc(sizeof(Polynomial));
    if (!poly) {
        if (err) *err = POLYNOMIAL_MEM_ALLOC_FAIL;
        return NULL;
    }
    
    poly->coefficients = malloc((degree + 1) * sizeof(void*));
    if (!poly->coefficients) {
        free(poly);
        if (err) *err = POLYNOMIAL_MEM_ALLOC_FAIL;
        return NULL;
    }
    
    for (int i = 0; i <= degree; i++) {
        poly->coefficients[i] = malloc(typeInfo->size);
        if (!poly->coefficients[i]) {
            for (int j = 0; j < i; j++) free(poly->coefficients[j]);
            free(poly->coefficients);
            free(poly);
            if (err) *err = POLYNOMIAL_MEM_ALLOC_FAIL;
            return NULL;
        }
        memset(poly->coefficients[i], 0, typeInfo->size);
    }
    
    poly->degree = degree;
    poly->typeInfo = typeInfo;
    if (err) *err = POLYNOMIAL_OK;
    return poly;
}

bool poly_is_equal(const Polynomial* a, const Polynomial* b) {
    if (!a || !b) return false;

    if (a->degree != b->degree || a->typeInfo != b->typeInfo) return false;

    for (int i = 0; i <= a->degree; i++) {
        if (a->typeInfo == GetIntTypeInfo()) {
            if (*(int*)a->coefficients[i] != *(int*)b->coefficients[i]) return false;
        } else if (a->typeInfo == GetComplexTypeInfo()) {
            const Complex* ca = a->coefficients[i];
            const Complex* cb = b->coefficients[i];
            if (fabs(ca->real - cb->real) > 1e-6 || fabs(ca->imag - cb->imag) > 1e-6) return false;
        }
    }

    return true;
}

Polynomial* poly_create_with_coeffs(TypeInfo* typeInfo, int degree, const void* coeffs, PolynomialError* err) {
    Polynomial* poly = poly_create(typeInfo, degree, err);
    if (!poly) return NULL;
    
    for (int i = 0; i <= degree; i++) {
        memcpy(poly->coefficients[i], (const char*)coeffs + i * typeInfo->size, typeInfo->size);
    }
    
    if (err) *err = POLYNOMIAL_OK;
    return poly;
}

void poly_free(Polynomial* poly) {
    if (!poly) return;
    for (int i = 0; i <= poly->degree; i++) {
        free(poly->coefficients[i]);
    }
    free(poly->coefficients);
    free(poly);
}

PolynomialError poly_add(const Polynomial* a, const Polynomial* b, Polynomial* result) {
    if (!a || !b || !result) return POLYNOMIAL_NULL_PTR;
    if (a->typeInfo != b->typeInfo || a->typeInfo != result->typeInfo) 
        return POLYNOMIAL_TYPE_MISMATCH;
    
    int max_degree = a->degree > b->degree ? a->degree : b->degree;
    if (result->degree < max_degree) return POLYNOMIAL_INVALID_DEGREE;
    
    for (int i = 0; i <= max_degree; i++) {
        if (i <= a->degree && i <= b->degree) {
            a->typeInfo->add(a->coefficients[i], b->coefficients[i], result->coefficients[i]);
        }
        else if (i <= a->degree) {
            memcpy(result->coefficients[i], a->coefficients[i], a->typeInfo->size);
        }
        else {
            memcpy(result->coefficients[i], b->coefficients[i], b->typeInfo->size);
        }
    }
    return POLYNOMIAL_OK;
}

PolynomialError poly_multiply(const Polynomial* a, const Polynomial* b, Polynomial* result) {
    if (!a || !b || !result) return POLYNOMIAL_NULL_PTR;
    if (a->typeInfo != b->typeInfo || a->typeInfo != result->typeInfo)
        return POLYNOMIAL_TYPE_MISMATCH;
    if (result->degree < a->degree + b->degree)
        return POLYNOMIAL_INVALID_DEGREE;
    
    for (int i = 0; i <= result->degree; i++) {
        memset(result->coefficients[i], 0, result->typeInfo->size);
    }
    
    for (int i = 0; i <= a->degree; i++) {
        for (int j = 0; j <= b->degree; j++) {
            void* temp = malloc(result->typeInfo->size);
            if (!temp) return POLYNOMIAL_MEM_ALLOC_FAIL;
            
            a->typeInfo->multiply(a->coefficients[i], b->coefficients[j], temp);
            
            void* sum = malloc(result->typeInfo->size);
            if (!sum) {
                free(temp);
                return POLYNOMIAL_MEM_ALLOC_FAIL;
            }
            
            a->typeInfo->add(result->coefficients[i+j], temp, sum);
            memcpy(result->coefficients[i+j], sum, result->typeInfo->size);
            
            free(temp);
            free(sum);
        }
    }
    return POLYNOMIAL_OK;
}

PolynomialError poly_scalar_multiply(const Polynomial* poly, const void* scalar, Polynomial* result) {
    if (!poly || !result) return POLYNOMIAL_NULL_PTR;
    if (poly->typeInfo != result->typeInfo) return POLYNOMIAL_TYPE_MISMATCH;
    if (result->degree < poly->degree) return POLYNOMIAL_INVALID_DEGREE;

    for (int i = 0; i <= poly->degree; i++) {
        poly->typeInfo->multiplyScalar(poly->coefficients[i], scalar, result->coefficients[i]);
    }

    for (int i = poly->degree + 1; i <= result->degree; i++) {
        memset(result->coefficients[i], 0, result->typeInfo->size);
    }

    return POLYNOMIAL_OK;
}

PolynomialError poly_evaluate(const Polynomial* poly, const void* x, void* result) {
    if (!poly || !x || !result) return POLYNOMIAL_NULL_PTR;

    memset(result, 0, poly->typeInfo->size);

    void* x_power = malloc(poly->typeInfo->size);
    void* term = malloc(poly->typeInfo->size);
    if (!x_power || !term) {
        free(x_power);
        free(term);
        return POLYNOMIAL_MEM_ALLOC_FAIL;
    }

    if (poly->typeInfo == GetIntTypeInfo()) {
        *(int*)x_power = 1;
    } else {
        Complex* cx = (Complex*)x_power;
        cx->real = 1.0;
        cx->imag = 0.0;
    }

    for (int i = 0; i <= poly->degree; i++) {
        poly->typeInfo->evaluate(poly->coefficients[i], x_power, term);
        poly->typeInfo->add(result, term, result);
        
        if (i < poly->degree) {
            void* temp = malloc(poly->typeInfo->size);
            if (!temp) {
                free(x_power);
                free(term);
                return POLYNOMIAL_MEM_ALLOC_FAIL;
            }
            poly->typeInfo->multiply(x_power, x, temp);
            memcpy(x_power, temp, poly->typeInfo->size);
            free(temp);
        }
    }

    free(x_power);
    free(term);
    return POLYNOMIAL_OK;
}

void poly_print(const Polynomial* poly) {
    if (!poly) {
        printf("Null polynomial\n");
        return;
    }
    
    int first = 1;
    for (int i = poly->degree; i >= 0; i--) {
        if (!first) {
            printf(" + ");
        }
        poly->typeInfo->print(poly->coefficients[i]);
        if (i > 0) printf("x");
        if (i > 1) printf("^%d", i);
        first = 0;
    }
    printf("\n");
}
