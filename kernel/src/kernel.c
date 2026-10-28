// bind every kernel module here

#include <proto.h>
#include <tests.h>
#include <segl.h>

typedef t_Proto proto;
typedef struct{
    void (*bomb)();
    _Bool (*assert)(char* expected, char* real, int size);
    int (*test)(t_Test_Result* (*fn)(), char* name, _Bool silent);
}testing;
