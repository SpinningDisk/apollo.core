#ifndef SOCKET_H
#define SOCKET_H
#include <Python.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>

#define SOCKET_VER 1.0


//forward
struct s_Socket;

// main
typedef enum {
    // msgpack  signals
    SOCKET_HEAD_HEADER              = 0x00,  // nil

    SOCKET_HEAD_FINISH              = 0x01,  // some special sauce
    SOCKET_HEAD_FALSE               = 0x02,  // false
    SOCKET_HEAD_TRUE                = 0x03,  // true

    SOCKET_HEAD_BIN_8               = 0x04,
    SOCKET_HEAD_BIN_16              = 0x05,
    SOCKET_HEAD_BIN_32              = 0x06,

    SOCKET_HEAD_EXT_8               = 0x07,
    SOCKET_HEAD_EXT_16              = 0x08,
    SOCKET_HEAD_EXT_32              = 0x09,

    SOCKET_HEAD_FLOAT               = 0x0a,
    SOCKET_HEAD_DOUBLE              = 0x0b,
    SOCKET_HEAD_UINT_8              = 0x0c,
    SOCKET_HEAD_UINT_16             = 0x0d,
    SOCKET_HEAD_UINT_32             = 0x0e,
    SOCKET_HEAD_UINT_64             = 0x0f,
    SOCKET_HEAD_INT_8               = 0x10,
    SOCKET_HEAD_INT_16              = 0x11,
    SOCKET_HEAD_INT_32              = 0x12,
    SOCKET_HEAD_INT_64              = 0x13,

    SOCKET_HEAD_FIXEXT_1            = 0x14,
    SOCKET_HEAD_FIXEXT_2            = 0x15,
    SOCKET_HEAD_FIXEXT_4            = 0x16,
    SOCKET_HEAD_FIXEXT_8            = 0x17,
    SOCKET_HEAD_FIXEXT_16           = 0x18,

    SOCKET_HEAD_STR_8               = 0x19, // str8
    SOCKET_HEAD_STR_16              = 0x1a, // str16
    SOCKET_HEAD_STR_32              = 0x1b, // str32
    SOCKET_HEAD_ARRAY_16            = 0x1c,
    SOCKET_HEAD_ARRAY_32            = 0x1d,
    SOCKET_HEAD_MAP_16              = 0x1e,
    SOCKET_HEAD_MAP_32              = 0x1f,

    // custom
    SOCKET_HEAD_EMPTY               = 0x20,
}t_Parser_Token_Types;
typedef struct s_Parser_Token{
    t_Parser_Token_Types type;
    char* content;
    size_t empty;
    size_t size;
    bool dynamic;
}t_Parser_Token;
typedef struct s_Parser_State{
    char* pos;
    t_Parser_Token* tokens;
    size_t tokenCount;
    size_t tokenAllocated;
    struct s_Parser_State (*parse)(char*);
}t_Parser_State;
typedef enum e_Status{
    // "normal codes"
    OK,
    HEARTBEAT,
    NOT_MODIFIED,

    // User definied
    SIG_0,
    SIG_1,
    SIG_2,
    SIG_3,
    SIG_4,
    SIG_5,
    SIG_6,
    SIG_7,
    SIG_8,
    SIG_9,

    // Error
    INVALID_SCHEME,
    FORBIDDEN,
    REJECTED,
    NOT_FOUND,
    INTERNAL_ERROR,
    TIMEOUT,
}t_Status;
typedef struct s_Response{
    t_Status sig;
    char* message;
    size_t MessageLength;
}t_Response; 
typedef struct s_Socket{
    int socketFd;
    struct sockaddr_un addr;
    char* path;
    void** io;

    t_Response* (*get)(struct s_Socket* sock);
    t_Status (*send)(struct s_Socket* sock, void* bytes);
}t_Socket;


static t_Response* init_response(t_Response* r);
t_Socket* init_server(t_Socket* server, const char*, unsigned int ports);
t_Socket* init_client(t_Socket* client, const char*);


static t_Response* get_next(t_Socket* s);
static t_Status socket_send(t_Socket* s, void* data);

#endif
