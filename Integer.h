#ifndef INTEGER_H
#define INTEGER_H

#include "TypeInfo.h"

extern TypeInfo* INT_TYPE_INFO;

void int_add(const void*, const void*, void*);
void int_multiply(const void*, const void*, void*);
void int_multiply_scalar(const void*, const void*, void*);
void int_evaluate(const void*, const void*, void*);
void int_print(const void*);
TypeInfo* GetIntTypeInfo();

#endif