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
    printf("Test PASSED: Creation works correctly.\n\n");
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

    Polynomial* expectedPoly = poly_create_with_coeffs(GetComplexTypeInfo(), 1, expected, &err);

    if (!poly_is_equal(expectedPoly, sum)) {
        printf("Test FAILED:\n");
    } else {
        printf("Test PASSED:\n");
    }

    // Выводим ожидаемый и фактический результат всегда
    printf("Expected Result: "); poly_print(expectedPoly); printf("\n");
    printf("Actual Result: "); poly_print(sum); printf("\n");

    poly_free(p1);
    poly_free(p2);
    poly_free(sum);
    poly_free(expectedPoly);
    printf("\n");
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

    Polynomial* expectedPoly = poly_create_with_coeffs(GetIntTypeInfo(), 2, expected, &err);

    if (!poly_is_equal(expectedPoly, prod)) {
        printf("Test FAILED:\n");
    } else {
        printf("Test PASSED:\n");
    }

    printf("Expected Result: "); poly_print(expectedPoly); printf("\n");
    printf("Actual Result: "); poly_print(prod); printf("\n");

    poly_free(p1);
    poly_free(p2);
    poly_free(prod);
    poly_free(expectedPoly);
    printf("\n");
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

    Polynomial* expectedPoly = poly_create_with_coeffs(GetIntTypeInfo(), 2, iexpected, &err);

    if (!poly_is_equal(expectedPoly, iresult)) {
        printf("Test FAILED:\n");
    } else {
        printf("Test PASSED:\n");
    }

    printf("Expected Result: "); poly_print(expectedPoly); printf("\n");
    printf("Actual Result: "); poly_print(iresult); printf("\n");

    Complex ccoeffs[] = {{1,1}, {2,0}};
    Complex cscalar = {0,1};
    Complex cexpected[] = {{-1,1}, {0,2}};

    Polynomial* cpoly = poly_create_with_coeffs(GetComplexTypeInfo(), 1, ccoeffs, &err);
    Polynomial* cresult = poly_create(GetComplexTypeInfo(), 1, &err);

    err = poly_scalar_multiply(cpoly, &cscalar, cresult);
    assert(err == POLYNOMIAL_OK);

    Polynomial* expectedCPoly = poly_create_with_coeffs(GetComplexTypeInfo(), 1, cexpected, &err);

    if (!poly_is_equal(expectedCPoly, cresult)) {
        printf("Test FAILED:\n");
    } else {
        printf("Test PASSED:\n");
    }

    printf("Expected Result: "); poly_print(expectedCPoly); printf("\n");
    printf("Actual Result: "); poly_print(cresult); printf("\n");

    poly_free(ipoly);
    poly_free(iresult);
    poly_free(cpoly);
    poly_free(cresult);
    poly_free(expectedPoly);
    poly_free(expectedCPoly);
    printf("\n");
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
    printf("Test PASSED:\n"); 
    printf("Expected Result: (%d)\n", iexpected); 
    printf("Actual Result: (%d)\n", iresult);
    printf("\n");
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

    printf("Expected Result: "); poly_print(onePolyI); printf("\n");
    printf("Actual Result: "); poly_print(sum); printf("\n");

    poly_free(zeroPolyI);
    poly_free(zeroPolyC);
    poly_free(onePolyI);
    poly_free(sum);
    printf("\n");
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

    int expectedBigCoeffs[] = {1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000};
    Polynomial* expectedScaledPoly = poly_create_with_coeffs(GetIntTypeInfo(), 9, expectedBigCoeffs, &err);

    if (!poly_is_equal(expectedScaledPoly, scaledBigPoly)) {
        printf("Test FAILED:\n");
    } else {
        printf("Test PASSED:\n");
    }

    printf("Expected Result: "); poly_print(expectedScaledPoly); printf("\n");
    printf("Actual Result: "); poly_print(scaledBigPoly); printf("\n");

    poly_free(bigPoly);
    poly_free(scaledBigPoly);
    poly_free(expectedScaledPoly);
    printf("\n");
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

    int expectedDiffSizeCoeffs[] = {4, 6, 5};
    Polynomial* expectedSumPoly = poly_create_with_coeffs(GetIntTypeInfo(), 2, expectedDiffSizeCoeffs, &err);

    if (!poly_is_equal(expectedSumPoly, sum)) {
        printf("Test FAILED:\n");
    } else {
        printf("Test PASSED:\n");
    }

    printf("Expected Result: "); poly_print(expectedSumPoly); printf("\n");
    printf("Actual Result: "); poly_print(sum); printf("\n");

    poly_free(smallPoly);
    poly_free(bigPoly);
    poly_free(sum);
    poly_free(expectedSumPoly);
    printf("\n");
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
    printf("All tests completed successfully!\n");
}
