#ifndef PROTO_H
#define PROTO_H
#define PROTO_VER 0.1
#include <stddef.h>
#include <socket.h>


// common
#define INSERT_CONST(buff, s1, s2, s3) strcpy(buff, s1);strcat(buff, s2);strcat(buff, s3);
#define errc(eval, code, fmt) fprintf(stderr, "%s: %s\n", fmt, strerror(code));exit(eval);

// forward
struct s_Parser_State;
struct s_Proto;


// specific
typedef struct s_Proto_Thread_Info{
    int id;
    void** io;
    pthread_mutex_t ready;
}t_Proto_Thread_Info;
typedef struct s_Proto{
    t_Parser_State* parser;
    t_Socket* server;
    
    pthread_t *threads;
    size_t threadCount;
    
    t_Proto_Thread_Info * results;
    pthread_mutex_t ready;
    
    // methods
    // s0
    t_Proto_Thread_Info* (*init_pti)(struct s_Proto_Thread_Info*);
    void (*thread)(struct s_Proto*, void* (*fn)(void*), void* arg);
    // s1 functions (closer to bare metal; the higher the simplicity s, the higher level everything gets
    void (*free)(struct s_Proto*);
    void (*connect)(struct s_Proto*, const char*);
    void (*check_segfaulted_thread)(struct s_Proto*);
    void* (*join_thread)(struct s_Proto*, pthread_t* thread);
    void (*read)(struct s_Proto*);
}t_Proto;


t_Parser_State* init_parser(t_Parser_State* self);
void free_parser(t_Parser_State* parser);

t_Proto_Thread_Info* init_pti(t_Proto_Thread_Info* pti);
static void free_pti(t_Proto_Thread_Info* pti);
t_Proto* init_proto(t_Proto* self, const char* appName);
static void free_proto(t_Proto* proto);

static void proto_thread(t_Proto* proto, void* (*fn)(void*), void* arg);
static void proto_connect(t_Proto* proto, const char* appName);
static void check_segfaulted_thread(t_Proto* proto);
static void* proto_join_thread(t_Proto* proto, pthread_t* thread);
static void proto_read(t_Proto* proto);

static t_Parser_State parse(char* bytes);

#endif
