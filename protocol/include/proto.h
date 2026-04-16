#ifndef PROTO_H
#define PROTO_H
#define PROTO_VER 1.0

#include <Python.h>


typedef struct s_ProtoState{
    PyObject** objects;
    unsigned int object_count;
    unsigned int object_count_allocated;
    
    PyObject** modules;
    unsigned int modules_count;
    unsigned int modules_count_allocated;
    
    PyObject*** module_cache;
    unsigned int* module_cache_index;
    
    t_Server server;
}t_ProtoState;

int getStrIndex(char* elm, char** arr, unsigned int arr_length);
t_ProtoState* _initProto_(const char* server_name);
t_ProtoState* answer(t_ProtoState *proto);

#endif