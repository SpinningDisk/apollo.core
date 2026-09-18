#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <proto.h>
#include <tests.h>
// #include <socket>


// utils
void print_trace(t_Function_Trace* trace){
    printf("<trace>@%p(%s(%s))", trace->fn, trace->name, trace->args);
    if(trace->child!=NULL){
        print_trace(trace->child);
    }
    return;
}
void printt(t_Test_Result* res){
    printf("<tests>@%s\t%s\x1b[0m", res->name, res->code==0?"\x1b[30;1mpassed":"\x1b[31;1mfailed");
    if(res->code!=0 && res->code!=127){
        printf("\x1b[31;1m (%s; code=%d)\x1b[0m", res->reason, res->code);
    }else if (res->code==127){
        printf("\x1b[31;1m (process crashed)\x1b[0m");
    }
    printf("\n");

}
_Bool arr_assert(char* expected, char* real, int size){
    if((expected==NULL)||(real==NULL)){return false;}
    for(int i=0; i<size; i++){
        if(expected[i]!=real[i]){
            return false;
        }
    }
    return true;
}
#define ASSERT(expected, real) expected==real?true:false


// test cases
// test tests

static void crash_function(){
    char* a = NULL;
    a[0] = 0;
    return;
}
t_Test_Result* test_crash(){
    t_Test_Result* res = malloc(sizeof(t_Test_Result));
    res-> code = ASSERT(test((t_Test_Result* (*)())crash_function, "\t[tests]@crash_function"), 127);

    if(res->code){
        res->reason = "test didn't crash";
    }
    res->name = "test";

    res->trace = malloc(sizeof(t_Function_Trace));
    res->trace->name = "test";
    res->trace->args = "crash_function, \"\t[tests]@crash_function\"";
    res->trace->fn = (void*)test;
    res->trace->child = malloc(sizeof(t_Function_Trace));
    res->trace->child->name = "crash_funtion";
    res->trace->child->args = "";
    res->trace->child->fn = crash_function;
    return res;
}


// proto_sockets
t_Test_Result* proto_socket(){
    t_Test_Result* res = malloc(sizeof(t_Test_Result));
    t_Socket* s = init_server(NULL, "/tmp/tests_suc.sock", 1);
    t_Socket* c = init_client(NULL, "/tmp/tests_suc.sock");


    res->name = "[proto]@init_server";
    if(s!=NULL&&c!=NULL){
        res->code = 0;
        res->reason = NULL;
    }else{
        res->code = 1;
        res->reason = "init failed";
    }

    res->trace = malloc(sizeof(t_Function_Trace));
    res->trace->name = "[proto]@init_server";
    res->trace->args = "NULL, \"/tmp/tests_suc.sock\", 1";
    res->trace->fn = (void*)init_server;
    res->trace->child = NULL;

    return res;
}



int test(t_Test_Result* (*fn)(), char* name){
    // new to pipes but I think what's happening is: create two fds, one for each process;
    // then create that pipe using pipe(pipe); then fork, and in the child, close the read end of the pipe,
    // write to the write end of the pipe, then close the write end of the pipe.
    // then in the parent, read from the read end of the pipe, and close the write end of the pipe.
    int pipefd[2];
    pipe(pipefd);
    int read_end = pipefd[0];
    int write_end = pipefd[1];

    pid_t pid = fork();
    if(pid==0){
        close(read_end);
        // write data to pipefd
        t_Test_Result *res = fn();
        write(write_end, res, sizeof(t_Test_Result));
        close(write_end);
        _exit(0);
    }
    // wait and read
    close(write_end);
    int code;
    waitpid(pid, &code, 0);
    t_Test_Result* res = malloc(sizeof(t_Test_Result));
    if(WIFEXITED(code)){
        read(read_end, res, sizeof(t_Test_Result));
    }else if(WIFSIGNALED(code)){
        res = malloc(sizeof(t_Test_Result));
        res->name = name;
        res->reason = "signaled";
        res->trace = NULL;
        res->code = 127;
    }
    return res->code;
}

int main(){
    test(proto_socket, "[proto]@init_server");
    test(test_crash, "[proto]@test_crash");
    return 0;
}


