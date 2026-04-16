#include "../include/socket.h"
#include "../include/proto.h"
#include <Python.h>
#include <stdio.h>

#define code_length 3
#define server_name "sockets/proto_test.sock"
int hanspeter(int* test){
    printf("\taddress to hanspeter: %p\n", test);
    return *test;
}
int main(){
    Py_Initialize();
    t_Server server = *prepareServer(server_name);
    PyObject* o = PyLong_FromLong(42);
    // PyObject* res = PyNumber_Add(o, o);
    // printf("local got %d+%d=%d\n", PyLong_AsLong(o), PyLong_AsLong(o), PyLong_AsLong(res));
    // printf("local being at 0x%x\n", o);
    // printf("local starting with 0x%x\n", *o);

    sendData(server, o);
    // getNextMessage(server->server_fd);
    return 0;
}