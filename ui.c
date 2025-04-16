#include "ui.h"
#include "PolynomialDefines.h"
#include "Integer.h"
#include "Complex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Polynomial* polynomials[MAX_POLYNOMIALS] = {NULL};
static int poly_count = 0;

void print_main_menu() {
    printf("\n=== Polynomial Calculator ===\n");
    printf("1. Create polynomial\n");
    printf("2. Print all polynomials\n");
    printf("3. Delete polynomial\n");
    printf("4. Polynomial operations\n");
    printf("5. Run tests\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

void print_operations_menu() {
    printf("\n=== Polynomial Operations ===\n");
    printf("1. Add polynomials\n");
    printf("2. Multiply polynomials\n");
    printf("3. Multiply polynomial by scalar\n");
    printf("4. Evaluate polynomial\n");
    printf("5. Return to main menu\n");
    printf("Enter your choice: ");
}

void print_polynomials_list() {
    if (poly_count == 0) {
        printf("No polynomials available\n");
        return;
    }
    
    printf("\nAvailable polynomials:\n");
    for (int i = 0; i < poly_count; i++) {
        if (polynomials[i] != NULL) {
            printf("%d: ", i+1);
            poly_print(polynomials[i]);
        }
    }
}

void create_int_polynomial() {
    int degree;
    printf("Enter polynomial degree: ");
    if (scanf("%d", &degree) != 1 || degree < 0) {
        printf("Invalid degree\n");
        while(getchar() != '\n');
        return;
    }
    
    PolynomialError err;
    Polynomial* poly = poly_create(GetIntTypeInfo(), degree, &err);
    if (err != POLYNOMIAL_OK) {
        printf("Error: %s\n", polynomial_error_msg(err));
        return;
    }
    
    if (poly_count >= MAX_POLYNOMIALS) {
        printf("Maximum number of polynomials reached\n");
        poly_free(poly);
        return;
    }
    
    printf("Enter %d integer coefficients: ", degree+1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%d", (int*)poly->coefficients[i]) != 1) {
            printf("Invalid input\n");
            poly_free(poly);
            while(getchar() != '\n');
            return;
        }
    }
    
    polynomials[poly_count++] = poly;
    printf("Polynomial created successfully\n");
}

void create_complex_polynomial() {
    int degree;
    printf("Enter polynomial degree: ");
    if (scanf("%d", &degree) != 1 || degree < 0) {
        printf("Invalid degree\n");
        while(getchar() != '\n');
        return;
    }
    
    PolynomialError err;
    Polynomial* poly = poly_create(GetComplexTypeInfo(), degree, &err);
    if (err != POLYNOMIAL_OK) {
        printf("Error: %s\n", polynomial_error_msg(err));
        return;
    }
    
    if (poly_count >= MAX_POLYNOMIALS) {
        printf("Maximum number of polynomials reached\n");
        poly_free(poly);
        return;
    }
    
    printf("Enter %d complex coefficients (real imag): ", degree+1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf %lf", 
              &((Complex*)poly->coefficients[i])->real,
              &((Complex*)poly->coefficients[i])->imag) != 2) {
            printf("Invalid input\n");
            poly_free(poly);
            while(getchar() != '\n');
            return;
        }
    }
    
    polynomials[poly_count++] = poly;
    printf("Polynomial created successfully\n");
}

void create_polynomial_menu() {
    int choice;
    do {
        printf("\nCreate Polynomial:\n");
        printf("1. Integer polynomial\n");
        printf("2. Complex polynomial\n");
        printf("3. Back to main menu\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1: create_int_polynomial(); break;
            case 2: create_complex_polynomial(); break;
            case 3: return;
            default: printf("Invalid choice\n");
        }
    } while (choice != 3);
}

void delete_polynomial() {
    print_polynomials_list();
    if (poly_count == 0) return;
    
    int num;
    printf("Enter polynomial number to delete: ");
    if (scanf("%d", &num) != 1 || num < 1 || num > poly_count) {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }
    
    num--;
    poly_free(polynomials[num]);
    for (int i = num; i < poly_count-1; i++) {
        polynomials[i] = polynomials[i+1];
    }
    polynomials[--poly_count] = NULL;
    printf("Polynomial deleted successfully\n");
}

void add_polynomials() {
    if (poly_count < 2) {
        printf("You need at least 2 polynomials\n");
        return;
    }
    
    print_polynomials_list();
    int num1, num2;
    
    printf("Select first polynomial (1-%d): ", poly_count);
    if (scanf("%d", &num1) != 1 || num1 < 1 || num1 > poly_count) {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }
    
    printf("Select second polynomial (1-%d): ", poly_count);
    if (scanf("%d", &num2) != 1 || num2 < 1 || num2 > poly_count) {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }
    
    Polynomial* p1 = polynomials[num1-1];
    Polynomial* p2 = polynomials[num2-1];
    PolynomialError err;
    
    Polynomial* result = poly_create(p1->typeInfo, 
        p1->degree > p2->degree ? p1->degree : p2->degree, &err);
    if (!result) return;
    
    err = poly_add(p1, p2, result);
    if (err != POLYNOMIAL_OK) {
        printf("Error: %s\n", polynomial_error_msg(err));
        poly_free(result);
        return;
    }
    
    printf("Result: ");
    poly_print(result);
    
    if (poly_count < MAX_POLYNOMIALS) {
        polynomials[poly_count++] = result;
    } else {
        printf("Cannot save result - maximum reached\n");
        poly_free(result);
    }
}

void multiply_polynomials() {
    if (poly_count < 2) {
        printf("You need at least 2 polynomials\n");
        return;
    }
    
    print_polynomials_list();
    int num1, num2;
    
    printf("Select first polynomial (1-%d): ", poly_count);
    if (scanf("%d", &num1) != 1 || num1 < 1 || num1 > poly_count) {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }
    
    printf("Select second polynomial (1-%d): ", poly_count);
    if (scanf("%d", &num2) != 1 || num2 < 1 || num2 > poly_count) {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }
    
    Polynomial* p1 = polynomials[num1-1];
    Polynomial* p2 = polynomials[num2-1];
    PolynomialError err;
    
    Polynomial* result = poly_create(p1->typeInfo, p1->degree + p2->degree, &err);
    if (!result) return;
    
    err = poly_multiply(p1, p2, result);
    if (err != POLYNOMIAL_OK) {
        printf("Error: %s\n", polynomial_error_msg(err));
        poly_free(result);
        return;
    }
    
    printf("Result: ");
    poly_print(result);
    
    if (poly_count < MAX_POLYNOMIALS) {
        polynomials[poly_count++] = result;
    } else {
        printf("Cannot save result - maximum reached\n");
        poly_free(result);
    }
}

void multiply_by_scalar() {
    print_polynomials_list();
    if (poly_count == 0) return;
    
    int num;
    printf("Select polynomial to multiply (1-%d): ", poly_count);
    if (scanf("%d", &num) != 1 || num < 1 || num > poly_count) {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }
    
    Polynomial* poly = polynomials[num-1];
    PolynomialError err;
    
    Polynomial* result = poly_create(poly->typeInfo, poly->degree, &err);
    if (!result) return;
    
    if (poly->typeInfo == GetIntTypeInfo()) {
        int scalar;
        printf("Enter integer scalar: ");
        if (scanf("%d", &scalar) != 1) {
            printf("Invalid input\n");
            poly_free(result);
            while(getchar() != '\n');
            return;
        }
        err = poly_scalar_multiply(poly, &scalar, result);
    } else {
        Complex scalar;
        printf("Enter complex scalar (real imag): ");
        if (scanf("%lf %lf", &scalar.real, &scalar.imag) != 2) {
            printf("Invalid input\n");
            poly_free(result);
            while(getchar() != '\n');
            return;
        }
        err = poly_scalar_multiply(poly, &scalar, result);
    }
    
    if (err != POLYNOMIAL_OK) {
        printf("Error: %s\n", polynomial_error_msg(err));
        poly_free(result);
        return;
    }
    
    printf("Result: ");
    poly_print(result);
    
    if (poly_count < MAX_POLYNOMIALS) {
        polynomials[poly_count++] = result;
    } else {
        printf("Cannot save result - maximum reached\n");
        poly_free(result);
    }
}

void evaluate_polynomial() {
    print_polynomials_list();
    if (poly_count == 0) return;
    
    int num;
    printf("Select polynomial to evaluate (1-%d): ", poly_count);
    if (scanf("%d", &num) != 1 || num < 1 || num > poly_count) {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }
    
    Polynomial* poly = polynomials[num-1];
    
    if (poly->typeInfo == GetIntTypeInfo()) {
        int x, res;
        printf("Enter integer x value: ");
        if (scanf("%d", &x) != 1) {
            printf("Invalid input\n");
            while(getchar() != '\n');
            return;
        }
        PolynomialError err = poly_evaluate(poly, &x, &res);
        if (err != POLYNOMIAL_OK) {
            printf("Error: %s\n", polynomial_error_msg(err));
            return;
        }
        printf("Result: %d\n", res);
    } else {
        Complex x, res;
        printf("Enter complex x value (real imag): ");
        if (scanf("%lf %lf", &x.real, &x.imag) != 2) {
            printf("Invalid input\n");
            while(getchar() != '\n');
            return;
        }
        PolynomialError err = poly_evaluate(poly, &x, &res);
        if (err != POLYNOMIAL_OK) {
            printf("Error: %s\n", polynomial_error_msg(err));
            return;
        }
        printf("Result: ");
        complex_print(&res);
        printf("\n");
    }
}

void run_operations_menu() {
    if (poly_count == 0) {
        printf("No polynomials available. Please create polynomials first.\n");
        return;
    }
    
    int choice;
    do {
        print_operations_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1: add_polynomials(); break;
            case 2: multiply_polynomials(); break;
            case 3: multiply_by_scalar(); break;
            case 4: evaluate_polynomial(); break;
            case 5: return;
            default: printf("Invalid choice\n");
        }
    } while (choice != 5);
}

void run_main_menu() {
    int choice;
    do {
        print_main_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1: create_polynomial_menu(); break;
            case 2: print_polynomials_list(); break;
            case 3: delete_polynomial(); break;
            case 4: run_operations_menu(); break;
            case 5: run_all_tests(); break;
            case 6: break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 6);
    
    // Cleanup
    for (int i = 0; i < poly_count; i++) {
        if (polynomials[i] != NULL) {
            poly_free(polynomials[i]);
        }
    }
}