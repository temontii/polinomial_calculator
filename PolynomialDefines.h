#ifndef POLYNOMIAL_DEFINES_H
#define POLYNOMIAL_DEFINES_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    POLYNOMIAL_OK = 0,
    POLYNOMIAL_MEM_ALLOC_FAIL = 100,
    POLYNOMIAL_NULL_PTR = 200,
    POLYNOMIAL_INVALID_DEGREE = 300,
    POLYNOMIAL_TYPE_MISMATCH = 400,
    POLYNOMIAL_INVALID_INPUT = 500,
    POLYNOMIAL_CALC_ERROR = 600
} PolynomialError;

static const char* polynomial_error_msg(PolynomialError err) {
    switch(err) {
        case POLYNOMIAL_OK: return "Success";
        case POLYNOMIAL_MEM_ALLOC_FAIL: return "Memory allocation failed";
        case POLYNOMIAL_NULL_PTR: return "Null pointer encountered";
        case POLYNOMIAL_INVALID_DEGREE: return "Invalid polynomial degree";
        case POLYNOMIAL_TYPE_MISMATCH: return "Type mismatch";
        case POLYNOMIAL_INVALID_INPUT: return "Invalid input";
        case POLYNOMIAL_CALC_ERROR: return "Calculation error";
        default: return "Unknown error";
    }
}

#endif