#ifndef TESTS_H
#define TESTS_H

#include "Polynomial.h"

typedef struct {
    const char* description;
    Polynomial* (*setup)();
    void (*test_func)(Polynomial*);
    void (*cleanup)(Polynomial*);
} PolynomialTest;

void run_all_tests();
void print_test_details(const char* operation, const Polynomial* expected, const Polynomial* actual, PolynomialError status);
void test_int_poly_creation();
void test_zero_and_minimal_polynomials();
void test_large_numbers_and_high_degrees();
void test_diff_size_polynomials();


#endif
