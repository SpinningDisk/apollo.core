#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <socket.h>
#include <proto.h>
#include <tests.h>
#define TEST_SPACE 45

// utils
void print_trace(t_Function_Trace* trace){
    printf("<trace>@%p(%s(%s))", trace->fn, trace->name, trace->args);
    if(trace->child!=NULL){
        print_trace(trace->child);
    }
    return;
}
#define PRINTS printf("\e[%dG", TEST_SPACE);
void printt(t_Test_Result* res){
    printf("<tests>@%s\t", res->name);
    PRINTS;
    printf("%s\x1b[0m", res->code==0?"\x1b[30;1mpassed":"\x1b[31;1mfailed");
    if(res->reason[0]!=NULL){
        printf("\x1b[31;1m (%s; code=%d)\x1b[0m ", res->reason, res->code);
    }else{
        if(res->code!=0 && res->code!=127){
            printf("\x1b[31;1m (%s; code=%d)\x1b[0m", res->reason, res->code);
        }else if (res->code==127){
            printf("\x1b[31;1m (process crashed)\x1b[0m");
        }
    }
    printf("\n");
    return;
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
static t_Test_Result* init_test(char* name){
    t_Test_Result* res = malloc(sizeof(t_Test_Result));
    res->name = name;
    res->code = 0;
    res->reason[0] = (char)0;
    return res;
}


// test cases
// test tests
static void crash_function(){
    char* a = NULL;
    a[0] = 0;
    return;
}
t_Test_Result* test_crash(){
    t_Test_Result* res = malloc(sizeof(t_Test_Result));
    res-> code = !(ASSERT(test((t_Test_Result* (*)())crash_function, "\t[tests]@crash_function", true), 127));

    if(res->code){
        strncpy(res->reason, "test didn't crash", 1024);
    }
    res->name = "[tests]@test";

    res->trace.name = "[tests]@test";
    res->trace.args = "crash_function, \"\t[tests]@crash_function\"";
    res->trace.fn = (void*)test;
    res->trace.child = malloc(sizeof(t_Function_Trace));
    res->trace.child->name = "[tests]@crash_funtion";
    res->trace.child->args = "";
    res->trace.child->fn = crash_function;
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
    }else{
        res->code = 1;
        strncpy(res->reason, "init failed", 1024);
    }

    res->trace.name = "[proto]@init_server";
    res->trace.args = "NULL, \"/tmp/tests_suc.sock\", 1";
    res->trace.fn = (void*)init_server;
    res->trace.child = NULL;

    return res;
}

// proto
static void* return_42(void* arg){
    char** carg = arg;
    carg[0] = (char*)malloc(sizeof(char)*2);

    carg[0][0] = 42;
    carg[0][1] = 0;
    return arg;
}
t_Test_Result* proto_init(){
    t_Test_Result* res = init_test("[proto]@init_proto");
    t_Proto* proto = init_proto(NULL, "tests.isolated");
    t_Proto_Thread_Info* zeros = calloc(2, sizeof(t_Proto_Thread_Info));
    res->code = !(1
    && arr_assert(proto->server->path, "/tmp/tests.isolated.sock!", strlen("/tmp/tests.isolated.sock!")-1));
    if(res->code){
        if(!arr_assert(proto->server->path, "/tmp/tests.isolated.sock", strlen("/tmp/tests.isolated.sock"))){
            char* path_cpy = malloc(1024);
            strncpy(path_cpy, proto->server->path, 1024);
            sprintf(res->reason, "path is not correct: %s vs %s\n", "tests.isolated", path_cpy);
            free(path_cpy);
        }
    }
    proto->free(proto);
    return res;
}
t_Test_Result* proto_thread(){
    t_Test_Result* res = init_test("[proto]@proto_thread");
    t_Proto* proto = init_proto(NULL, "tests.isolated");
    int id = proto->thread(proto, return_42, NULL);
    void* answer = proto->join_thread(proto, id);
    res->code = !(ASSERT(((char**)answer)[0][0], 42));
    if(res->code){
        sprintf(res->reason, "thread did not return the answer to live, death and everything, 42 but %d", *(int*)answer);
        printf("%s", res->reason);
    }
    proto->free(proto);
    return res;

}
t_Test_Result* proto_connect(){
    t_Test_Result* res = init_test("[proto]@proto_connect");
    t_Proto* proto = init_proto(NULL, "tests.isolated");
    proto->connect(proto, "tests.isolated");
    res->code = !(arr_assert(proto->server->path, "/tmp/tests.isolated.sock", strlen("/tmp/tests.isolated.sock")));
    if(res->code){
        strcpy(res->reason, "path is not correct: ");
        strcpy(res->reason+strlen(res->reason), "/tmp/tests.isolated.sock vs ");
        strncpy(res->reason+strlen(res->reason), proto->server->path, 1024-strlen(res->reason)-1);
    }
    proto->free(proto);
    return res;
}


int test(t_Test_Result* (*fn)(), char* name, _Bool silent){
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
        // res->reason = "signaled";
        res->code = 127;
    }
    if(!silent)
        printt(res);

    return res->code;
}

int main(){
    test(proto_socket, "[proto]@init_server", false);
    // test(test_crash, "[proto]@test_crash", false);
    test(proto_init, "[proto]@init_proto", false);
    test(proto_thread, "[proto]@proto_thread", false);
    test(proto_connect, "[proto]@proto_connect", false);
    return 0;
}


