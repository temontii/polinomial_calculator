#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <stdlib.h>

typedef void (*BinaryOp)(const void*, const void*, void*);
typedef void (*UnaryOp)(const void*);
typedef void (*EvalOp)(const void*, const void*, void*);

typedef struct {
    size_t size;
    BinaryOp add;
    BinaryOp multiply;
    BinaryOp multiplyScalar;
    EvalOp evaluate;
    UnaryOp print;
} TypeInfo;

#endif