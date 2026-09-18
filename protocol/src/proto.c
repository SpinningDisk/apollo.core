#include <stdlib.h>
#include <err.h>
#include <pthread.h>
#include <socket.h>
#include <proto.h>


static t_Parser_State parse(char* bytes){};

t_Parser_State* init_parser(t_Parser_State* self){
    if(self==NULL){
        self = malloc(sizeof(t_Parser_State));
    }
    self->pos = NULL;
    self->tokens = NULL;
    self->parse = parse;
    return self;
}
void free_parser(t_Parser_State* parser){
    free(parser->tokens);
    free(parser->pos);
    free(parser);
    return;
}

t_Proto_Thread_Info* init_pti(t_Proto_Thread_Info* pti){
    if(pti==NULL){
        pti = malloc(sizeof(t_Proto_Thread_Info));
    }
    pti->id = -1;
    pti->io = malloc(2*sizeof(void*));
    pti->io[0] = malloc(sizeof(char*));
    pti->io[1] = malloc(sizeof(char*));

    pthread_mutexattr_t    attr;
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&pti->ready, &attr);
    return pti;
}
static void free_pti(t_Proto_Thread_Info* pti){
    pthread_mutex_lock(&pti->ready);
    char* ptr = pti->io[1];
    free(ptr);
    // free((char*)pti->io[0]);
    // free((char*)pti->io[1]);
    free(pti->io);
    pthread_mutex_destroy(&pti->ready);
    free(pti);
}
t_Proto* init_proto(t_Proto* self, const char* appName){
    if(self==NULL){
        self = malloc(sizeof(t_Proto));
    }
    self->parser = init_parser(NULL);
    char path[100];
    #ifdef __linux__
        INSERT_CONST(path, "/tmp/", appName, ".sock");
    #endif

    self->server = init_server(self->server, path, 1);        // sanitizations problems

    self->threads = (pthread_t*)calloc(2, sizeof(pthread_t));
    self->threadCount = 0;
    self->results = (t_Proto_Thread_Info*)calloc(2, sizeof(t_Proto_Thread_Info));

    pthread_mutexattr_t    attr;
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&self->ready, &attr);

    // methods
    // s0
    self->init_pti = init_pti;
    self->thread = proto_thread;
    //s1
    self->free = free_proto;
    self->connect = proto_connect;
    self->join_thread = proto_join_thread;
    self->read = proto_read;
    return self;
}
static void free_proto(t_Proto* self){
    free_parser(self->parser);
    free(self->server->path);
    free(self->server);

    pthread_mutex_lock(&self->ready);
    pthread_mutex_unlock(&self->ready);

    free(self->threads);
    for(int i=0; i<self->threadCount; i++){
        free_pti(&self->results[i]);
    }
    // free(self->results);
    pthread_mutex_destroy(&self->ready);

    free(self);
    return;
}


static void proto_thread(t_Proto* Proto, void* (*fn)(void*), void* arg){
    // creates thread running function "fn" given argument "arg" and stores thread in "Proto->threads" along with some threading information and results in Proto->results
    #ifdef DEBUG
    printf("[Proto]:\tenter <proto_thread>@Proto.c\n");
    #endif

    pthread_attr_t attr;
    int c = pthread_attr_init(&attr);
    if(c!=0){errc(EXIT_FAILURE, c, "[Proto]@pthread_attr_init");}

    c = pthread_attr_setstacksize(&attr, 0x400000);
    if(c!=0){errc(EXIT_FAILURE, c, "[Proto]@pthread_attr_setstacksize");}

    #ifdef DEBUG
    printf("  locking for Proto @%p\n", Proto);
    #endif
    pthread_mutex_lock(&Proto->ready);
    Proto->threads = realloc(Proto->threads, sizeof(pthread_t)*(Proto->threadCount+1));
    Proto->results = realloc(Proto->results, sizeof(t_Proto_Thread_Info)*(Proto->threadCount+1));
    init_pti(&Proto->results[Proto->threadCount]);
    void** io = Proto->results[Proto->threadCount].io;
    io[1] = arg;

    c = pthread_create(&(Proto->threads[Proto->threadCount]), &attr, fn, io);
    if (c!=0){pthread_mutex_unlock(&Proto->ready); errc(EXIT_FAILURE, c, "[Proto]@pthread_create");}
    Proto->threadCount++;
    pthread_mutex_unlock(&Proto->ready);
    #ifdef DEBUG
    printf("unlocking for Proto @%p\n", Proto);
    #endif

    c = pthread_attr_destroy(&attr);
    if(c!=0){errc(EXIT_FAILURE, c, "[Proto]@pthread_attr_destroy");}
    return;

}
static void* _proto_connect(void* args){
    void** io = args;
    char* path = (char*)(io[1]);

    void* con = path;

    io[0] = malloc(sizeof(t_Socket));
    memcpy(io[0], con, sizeof(t_Socket));
    return io[0];
}
static void proto_connect(t_Proto* proto, const char* appName){
    proto->thread(proto, _proto_connect, (void*)appName);
    return;
}
static void* proto_join_thread(t_Proto* Proto, pthread_t* thread){
    void* result = NULL;
#ifdef DEBUG
    printf("[Proto@%p]:\tentering pJT for thread %d\n", Proto, *thread);
#endif
    char is_owner=(char)0;
    for(size_t i=0; i<Proto->threadCount; i++){
        if(thread==&(Proto->threads[i])){
            is_owner=1;
            break;
        }
    }
#ifdef DEBUG
    printf("[Proto@%p]:\tis owner of %d? %d\n", Proto, *thread, is_owner);
#endif
    if(is_owner){
        pthread_mutex_lock(&Proto->results[Proto->threadCount-1].ready);
        int c = pthread_join(*thread, Proto->results[Proto->threadCount-1].io);
        result = Proto->results[Proto->threadCount-1].io[0];
#ifdef DEBUG
        printf("[Proto@%p]:\tjoined %p with result %d stored to %p, possibly errored with %d\n", Proto, *thread, *(int*)result, result, c);
#endif
        pthread_mutex_unlock(&Proto->results[Proto->threadCount-1].ready);
    }
    return result;
}

static void proto_read(t_Proto* Proto){

    return;
}
