#include "tests.h"
#include "Polynomial.h"
#include "Integer.h"
#include "Complex.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>

#define EPSILON 1e-6

int complex_equals(const Complex* a, const Complex* b) {
    return fabs(a->real - b->real) < EPSILON && 
           fabs(a->imag - b->imag) < EPSILON;
}

void test_int_poly_creation() {
    printf("=== Testing integer polynomial creation ===\n");
    PolynomialError err;
    int coeffs[] = {1, 2, 3};
    Polynomial* p = poly_create_with_coeffs(GetIntTypeInfo(), 2, coeffs, &err);
    
    assert(err == POLYNOMIAL_OK);
    assert(p != NULL);
    assert(p->degree == 2);
    assert(*(int*)p->coefficients[0] == 1);
    assert(*(int*)p->coefficients[1] == 2);
    assert(*(int*)p->coefficients[2] == 3);
    
    poly_free(p);
    printf("Test passed!\n\n");
}

void test_complex_poly_addition() {
    printf("=== Testing complex polynomial addition ===\n");
    PolynomialError err;
    Complex coeffs1[] = {{1,1}, {2,0}};
    Complex coeffs2[] = {{3,-1}, {0,1}};
    Complex expected[] = {{4,0}, {2,1}};
    
    Polynomial* p1 = poly_create_with_coeffs(GetComplexTypeInfo(), 1, coeffs1, &err);
    Polynomial* p2 = poly_create_with_coeffs(GetComplexTypeInfo(), 1, coeffs2, &err);
    Polynomial* sum = poly_create(GetComplexTypeInfo(), 1, &err);
    
    err = poly_add(p1, p2, sum);
    assert(err == POLYNOMIAL_OK);
    
    for (int i = 0; i <= sum->degree; i++) {
        assert(complex_equals(sum->coefficients[i], &expected[i]));
    }
    
    poly_free(p1);
    poly_free(p2);
    poly_free(sum);
    printf("Test passed!\n\n");
}

void test_poly_multiplication() {
    printf("=== Testing polynomial multiplication ===\n");
    PolynomialError err;
    int coeffs1[] = {1, 2};
    int coeffs2[] = {3, 4};
    int expected[] = {3, 10, 8};
    
    Polynomial* p1 = poly_create_with_coeffs(GetIntTypeInfo(), 1, coeffs1, &err);
    Polynomial* p2 = poly_create_with_coeffs(GetIntTypeInfo(), 1, coeffs2, &err);
    Polynomial* prod = poly_create(GetIntTypeInfo(), 2, &err);
    
    err = poly_multiply(p1, p2, prod);
    assert(err == POLYNOMIAL_OK);
    
    for (int i = 0; i <= prod->degree; i++) {
        assert(*(int*)prod->coefficients[i] == expected[i]);
    }
    
    poly_free(p1);
    poly_free(p2);
    poly_free(prod);
    printf("Test passed!\n\n");
}

void test_scalar_multiplication() {
    printf("=== Testing scalar multiplication ===\n");
    PolynomialError err;
    
    int icoeffs[] = {1, 2, 3};
    int iscalar = 2;
    int iexpected[] = {2, 4, 6};
    
    Polynomial* ipoly = poly_create_with_coeffs(GetIntTypeInfo(), 2, icoeffs, &err);
    Polynomial* iresult = poly_create(GetIntTypeInfo(), 2, &err);
    
    err = poly_scalar_multiply(ipoly, &iscalar, iresult);
    assert(err == POLYNOMIAL_OK);
    
    for (int i = 0; i <= iresult->degree; i++) {
        assert(*(int*)iresult->coefficients[i] == iexpected[i]);
    }
    
    Complex ccoeffs[] = {{1,1}, {2,0}};
    Complex cscalar = {0,1};
    Complex cexpected[] = {{-1,1}, {0,2}};
    
    Polynomial* cpoly = poly_create_with_coeffs(GetComplexTypeInfo(), 1, ccoeffs, &err);
    Polynomial* cresult = poly_create(GetComplexTypeInfo(), 1, &err);
    
    err = poly_scalar_multiply(cpoly, &cscalar, cresult);
    assert(err == POLYNOMIAL_OK);
    
    for (int i = 0; i <= cresult->degree; i++) {
        assert(complex_equals(cresult->coefficients[i], &cexpected[i]));
    }
    
    poly_free(ipoly);
    poly_free(iresult);
    poly_free(cpoly);
    poly_free(cresult);
    printf("Test passed!\n\n");
}

void test_poly_evaluation() {
    printf("=== Testing polynomial evaluation ===\n");
    PolynomialError err;
    
    int icoeffs[] = {1, 2, 3};
    int ix = 2;
    int iexpected = 17;
    int iresult;
    
    Polynomial* ipoly = poly_create_with_coeffs(GetIntTypeInfo(), 2, icoeffs, &err);
    err = poly_evaluate(ipoly, &ix, &iresult);
    assert(err == POLYNOMIAL_OK);
    assert(iresult == iexpected);
    
    Complex ccoeffs[] = {{1,1}, {2,0}};
    Complex cx = {0,1};
    Complex cexpected = {1,3};
    Complex cresult;
    
    Polynomial* cpoly = poly_create_with_coeffs(GetComplexTypeInfo(), 1, ccoeffs, &err);
    err = poly_evaluate(cpoly, &cx, &cresult);
    assert(err == POLYNOMIAL_OK);
    
    assert(complex_equals(&cresult, &cexpected));
    
    poly_free(ipoly);
    poly_free(cpoly);
    printf("Test passed!\n\n");
}

void test_error_handling() {
    printf("=== Testing error handling ===\n");
    PolynomialError err;
    
    Polynomial* p = poly_create(NULL, 2, &err);
    assert(p == NULL);
    assert(err == POLYNOMIAL_NULL_PTR);
    
    p = poly_create(GetIntTypeInfo(), -1, &err);
    assert(p == NULL);
    assert(err == POLYNOMIAL_INVALID_DEGREE);
    
    int coeffs[] = {1, 2};
    Complex ccoeffs[] = {{1,1}};
    Polynomial* ipoly = poly_create_with_coeffs(GetIntTypeInfo(), 1, coeffs, &err);
    Polynomial* cpoly = poly_create_with_coeffs(GetComplexTypeInfo(), 1, ccoeffs, &err);
    Polynomial* result = poly_create(GetIntTypeInfo(), 1, &err);
    
    err = poly_add(ipoly, cpoly, result);
    assert(err == POLYNOMIAL_TYPE_MISMATCH);
    
    poly_free(ipoly);
    poly_free(cpoly);
    poly_free(result);
    printf("Test passed!\n\n");
}

void test_zero_and_minimal_polynomials() {
    printf("=== Testing zero and minimal-degree polynomials ===\n");
    PolynomialError err;

    int zCoeffs[] = {0}; 
    Polynomial* zeroPolyI = poly_create_with_coeffs(GetIntTypeInfo(), 0, zCoeffs, &err);
    assert(zeroPolyI != NULL);
    assert(err == POLYNOMIAL_OK);

    Complex zCoeffsC[] = {{0, 0}}; 
    Polynomial* zeroPolyC = poly_create_with_coeffs(GetComplexTypeInfo(), 0, zCoeffsC, &err);
    assert(zeroPolyC != NULL);
    assert(err == POLYNOMIAL_OK);

    int oneCoeffs[] = {1}; 
    Polynomial* onePolyI = poly_create_with_coeffs(GetIntTypeInfo(), 0, oneCoeffs, &err);
    assert(onePolyI != NULL);
    assert(err == POLYNOMIAL_OK);

    Polynomial* sum = poly_create(GetIntTypeInfo(), 0, &err);
    err = poly_add(zeroPolyI, onePolyI, sum);
    assert(err == POLYNOMIAL_OK);
    assert(*(int*)sum->coefficients[0] == 1); 

    poly_free(zeroPolyI);
    poly_free(zeroPolyC);
    poly_free(onePolyI);
    poly_free(sum);
    printf("Test passed!\n\n");
}

void test_large_numbers_and_high_degrees() {
    printf("=== Testing large numbers and high degrees ===\n");
    PolynomialError err;

    int bigCoeffs[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Polynomial* bigPoly = poly_create_with_coeffs(GetIntTypeInfo(), 9, bigCoeffs, &err);
    assert(bigPoly != NULL);
    assert(err == POLYNOMIAL_OK);

    int bigScalar = 1000000;
    Polynomial* scaledBigPoly = poly_create(GetIntTypeInfo(), 9, &err);
    err = poly_scalar_multiply(bigPoly, &bigScalar, scaledBigPoly);
    assert(err == POLYNOMIAL_OK);

    for (int i = 0; i <= bigPoly->degree; i++) {
        assert(*(int*)scaledBigPoly->coefficients[i] == bigCoeffs[i]*bigScalar);
    }

    poly_free(bigPoly);
    poly_free(scaledBigPoly);
    printf("Test passed!\n\n");
}

void test_diff_size_polynomials() {
    printf("=== Testing different-sized polynomials ===\n");
    PolynomialError err;

    int smallCoeffs[] = {1, 2};
    int bigCoeffs[] = {3, 4, 5};
    Polynomial* smallPoly = poly_create_with_coeffs(GetIntTypeInfo(), 1, smallCoeffs, &err);
    Polynomial* bigPoly = poly_create_with_coeffs(GetIntTypeInfo(), 2, bigCoeffs, &err);

    Polynomial* sum = poly_create(GetIntTypeInfo(), 2, &err);
    err = poly_add(smallPoly, bigPoly, sum);
    assert(err == POLYNOMIAL_OK);
    assert(*(int*)sum->coefficients[0] == 4);
    assert(*(int*)sum->coefficients[1] == 6);
    assert(*(int*)sum->coefficients[2] == 5);

    poly_free(smallPoly);
    poly_free(bigPoly);
    poly_free(sum);
    printf("Test passed!\n\n");
}

void run_all_tests() {
    test_int_poly_creation();
    test_complex_poly_addition();
    test_poly_multiplication();
    test_scalar_multiplication();
    test_poly_evaluation();
    test_zero_and_minimal_polynomials();
    test_large_numbers_and_high_degrees();
    test_diff_size_polynomials();
    test_error_handling();
    printf("All tests passed!\n");
}