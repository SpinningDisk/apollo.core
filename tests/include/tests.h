#ifndef TESTS_H
#define TESTS_H

// forward
struct s_Function_Trace;
// main
typedef struct s_Function_Trace{
    char* name;
    char* args;
    void* fn;

    struct s_Function_Trace* child;
}t_Function_Trace;
typedef struct s_Test_Result{
    char code;
    char* name;

    // on crash
    char* reason;
    t_Function_Trace* trace;
}t_Test_Result;


void print_trace(t_Function_Trace* trace);
void printt(t_Test_Result* res);
_Bool arr_assert(char* expected, char* real, int size);
#define ASSERT(expected, real) arr_assert(expected, real, sizeof(real))

// test cases
static void crash_function();
t_Test_Result* test_crash();
int test(t_Test_Result* (*fn)(), char* name);


t_Test_Result* proto_socket();

#endif
