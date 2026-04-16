#include <sys/socket.h>
#include <sys/un.h>
#include <Python.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>


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


t_Response getNextMessage(int server_fd){
    t_Response response;
    response.message = (char*)malloc(sizeof(char)*2048);
    memset(response.message, 0, 2048);
    listen(server_fd, 5);
    response.client_fd = accept(server_fd, NULL, NULL);
    
    int n = read(response.client_fd, response.message, 2048);
    response.message_length = n;
    response.message = realloc(response.message, n*sizeof(char));
    return response;
}

t_Server* prepareServer(const char* name){
    t_Server *server = (t_Server*)malloc(sizeof(t_Server));
    server->server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    unlink(name);
    memset(&server->addr, 0, sizeof(server->addr));
    server->addr.sun_family = AF_UNIX;
    strncpy(server->addr.sun_path, name, sizeof(server->addr.sun_path) - 1);
    printf("got here!\n>:3\n");
    bind(server->server_fd, (struct sockaddr*)&server->addr, sizeof(server->addr));
    return server;
}

// t_Status send_data(t_Server server, PyObject* o)
t_Status sendData(t_Server server, PyObject* o){
    printf("[SEND_DATA]@ENTRANCE\n");

    // PyObject* res = PyNumber_Add(o, o);
    // PyObject* res = PyLong_FromLong(69);
    printf("@%x\n", o);
    // printf("extern %d+%d=%d\n", PyLong_AsLong(o), PyLong_AsLong(o), PyLong_AsLong(res));
    // getNextMessage(server.server_fd);
    return OK;
}