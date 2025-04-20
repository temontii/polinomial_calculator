#ifndef POLYNOMIAL_DEFINES_H
#define POLYNOMIAL_DEFINES_H

#include <stdlib.h>
#include <string.h>

typedef enum {
    POLYNOMIAL_OK = 0,
    POLYNOMIAL_MEM_ALLOC_FAIL = 100,
    POLYNOMIAL_NULL_PTR = 200,
    POLYNOMIAL_INVALID_DEGREE = 300,
    POLYNOMIAL_TYPE_MISMATCH = 400,
    POLYNOMIAL_INVALID_INPUT = 500,
    POLYNOMIAL_CALC_ERROR = 600,
    POLYNOMIAL_NOT_EQUAL = 700,
    POLYNOMIAL_EQUAL = 0
} PolynomialError;

typedef struct {
    int code;
    const char* message;
} PolynomialErrorInfo;

static const PolynomialErrorInfo ERROR_TABLE[] = {
    {POLYNOMIAL_OK, "Success"},
    {POLYNOMIAL_MEM_ALLOC_FAIL, "Memory allocation failed"},
    {POLYNOMIAL_NULL_PTR, "Null pointer encountered"},
    {POLYNOMIAL_INVALID_DEGREE, "Invalid polynomial degree"},
    {POLYNOMIAL_TYPE_MISMATCH, "Type mismatch"},
    {POLYNOMIAL_INVALID_INPUT, "Invalid input"},
    {POLYNOMIAL_CALC_ERROR, "Calculation error"},
    {POLYNOMIAL_NOT_EQUAL, "Polynomials are not equal"},
    {POLYNOMIAL_EQUAL, "Polynomials are equal"}
};

static const char* polynomial_error_msg(PolynomialError err) {
    for (size_t i = 0; i < sizeof(ERROR_TABLE)/sizeof(ERROR_TABLE[0]); i++) {
        if (ERROR_TABLE[i].code == err) {
            return ERROR_TABLE[i].message;
        }
    }
    return "Unknown error";
}

#endif
