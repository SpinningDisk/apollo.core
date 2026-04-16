#ifndef SOCKET_H
#define SOCKET_H
#include <Python.h>

#define SOCKET_VER 1.0

struct sockaddr_un{};

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
    char* message;
    unsigned int message_length;
    int client_fd;
}t_Response; 
typedef struct s_Server{
    int server_fd;
    int client_fd;
    struct sockaddr_un addr;
}t_Server;

t_Response getNextMessage(int server_fd);
t_Server* prepareServer(const char* name);
t_Status sendData(t_Server, PyObject*);

#endif