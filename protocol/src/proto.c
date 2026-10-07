#include <stdlib.h>
#include <err.h>
#include <pthread.h>
#include <socket.h>
#include <proto.h>

// forward
static t_Parser_State parse(char* bytes){};
static void free_pti(t_Proto_Thread_Info* pti);
static void free_proto(t_Proto* proto);
static int proto_thread(t_Proto* proto, void* (*fn)(void*), void* arg);
static void proto_connect(t_Proto* proto, const char* appName);
static void check_segfaulted_thread(t_Proto* proto);
static void* proto_join_thread(t_Proto* proto, int index);
static void proto_read(t_Proto* proto);
static t_Parser_State parse(char* bytes);


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
    pti->io = malloc(2*sizeof(char*));
    pti->io[0] = malloc(sizeof(char)*1024);
    pti->io[1] = malloc(sizeof(char)*1024);

    pthread_mutexattr_t attr;
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&pti->ready, &attr);
    return pti;
}
static void free_pti(t_Proto_Thread_Info* pti){
    pthread_mutex_lock(&pti->ready);
    free(pti->io[1]);
    free(pti->io[0]);
    free(pti->io);
    pthread_mutex_destroy(&pti->ready);
    return;
}
static void xfree_pti(t_Proto_Thread_Info* pti){
    pthread_mutex_lock(&pti->ready);
    if(pti->io[1]!=NULL){free(pti->io[1]);free(pti->io[0]);}
    free(pti->io);
    pthread_mutex_unlock(&pti->ready);
    return;
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

    self->threads = (pthread_t*)calloc(PROTO_MAX_THREADS, sizeof(pthread_t));
    self->ptiPool = (t_Proto_Thread_Info*)calloc(PROTO_MAX_THREADS, sizeof(t_Proto_Thread_Info));
    for(size_t i=0; i<PROTO_MAX_THREADS; i++){init_pti(&self->ptiPool[i]);}
    self->freePtiSlots = (bool*)calloc(PROTO_MAX_THREADS, sizeof(bool));
    for(size_t i=0; i<PROTO_MAX_THREADS; i++){self->freePtiSlots[i] = true;}


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
    free(self->threads);
    for(size_t i=0; i<PROTO_MAX_THREADS; i++){xfree_pti(&self->ptiPool[i]);}
    free(self->ptiPool);
    pthread_mutex_unlock(&self->ready);
    pthread_mutex_destroy(&self->ready);
    return;
}


static int proto_thread(t_Proto* Proto, void* (*fn)(void*), void* arg){
    // creates thread running function "fn" given argument "arg" and stores thread in "Proto->threads" along with some threading information and results in Proto->results

    pthread_attr_t attr;
    int c = pthread_attr_init(&attr);
    if(c!=0){errc(EXIT_FAILURE, c, "[Proto]@pthread_attr_init");}

    c = pthread_attr_setstacksize(&attr, 0x400000);
    if(c!=0){errc(EXIT_FAILURE, c, "[Proto]@pthread_attr_setstacksize");}

    pthread_mutex_lock(&Proto->ready);
    char** io = NULL;
    size_t i=0;
    for(; i<PROTO_MAX_THREADS; i++){
        if(Proto->freePtiSlots[i]){
            Proto->freePtiSlots[i] = false;
            init_pti(&Proto->ptiPool[i]);
            io = Proto->ptiPool[i].io;
            break;
        }
    }
    if(io==NULL){errc(EXIT_FAILURE, 0, "[Proto]@no free PTI slots");}
    io[1] = arg;
    c = pthread_create(&(Proto->threads[i]), &attr, fn, io);
    if (c!=0){pthread_mutex_unlock(&Proto->ready); errc(EXIT_FAILURE, c, "[Proto]@pthread_create");}
    pthread_mutex_unlock(&Proto->ready);

    c = pthread_attr_destroy(&attr);
    if(c!=0){errc(EXIT_FAILURE, c, "[Proto]@pthread_attr_destroy");}
    return i;
}
static void* _proto_connect(void* args){
    /*char** io = args;
    char* path = io[1];

    void* con = path;

    io[0] = malloc(sizeof(char)*1024);*/
    // memcpy(io[0], con, sizeof(t_Socket));
    return args;
}
static void proto_connect(t_Proto* proto, const char* appName){
    int id = proto->thread(proto, _proto_connect, (void*)appName);   // TODO: return id from thread
    pthread_mutex_lock(&proto->ready);
    proto->join_thread(proto, id);
    pthread_mutex_unlock(&proto->ready);
    return;
}
static void* proto_join_thread(t_Proto* Proto, int index){
    pthread_mutex_lock(&Proto->ptiPool[index].ready);
    int c = pthread_join(Proto->threads[index], NULL);
    if(c!=0){errc(EXIT_FAILURE, c, "[Proto]@pthread_join");}
    char** result = malloc(sizeof(char*)*2);
    if(Proto->ptiPool[index].io[0]==NULL){result[0] = NULL;}else
        memcpy(result[0], Proto->ptiPool[index].io[0], strlen(Proto->ptiPool[index].io[0]));
    if(Proto->ptiPool[index].io[1]==NULL){result[1] = NULL;}else
        memcpy(result[1], Proto->ptiPool[index].io[1], strlen(Proto->ptiPool[index].io[1]));
    // this sucks, actually; theoretically, we should decrease by one but then we'd need to free pti stuff; aka I'd now need to implement the PTI pool; might do later

    pthread_mutex_unlock(&Proto->ptiPool[index].ready);
    return result;
}

static void proto_read(t_Proto* Proto){

    return;
}
