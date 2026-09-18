#include "../include/socket.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <Python.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>


static t_Parser_Token* init_token(t_Parser_Token* tok, t_Parser_Token_Types type){
    if(tok==NULL){
        tok = malloc(sizeof(t_Parser_Token));
    }
    tok->type = type;
    switch(type){
        case SOCKET_HEAD_HEADER:
            tok->empty = 0;
            tok->size = 0;
            break;
        case SOCKET_HEAD_FINISH:
            tok->empty = 0;
            tok->size = 0;
            break;
        case SOCKET_HEAD_FALSE:
            tok->empty = 0;
            tok->size = 0;
            break;
        case SOCKET_HEAD_TRUE:
            tok->empty = 0;
            tok->size = 0;
            break;
        case SOCKET_HEAD_BIN_8:
            tok->empty = 1;
            tok->size = 1;
            break;
        case SOCKET_HEAD_BIN_16:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_BIN_32:
            tok->empty = 3;
            tok->size = 3;
            break;
        case SOCKET_HEAD_EXT_8:
            tok->empty = 1;
            tok->size = 1;
            break;
        case SOCKET_HEAD_EXT_16:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_EXT_32:
            tok->empty = 4;
            tok->size = 4;
            break;
        case SOCKET_HEAD_FLOAT:
            tok->empty = 4;
            tok->size = 4;
            break;
        case SOCKET_HEAD_DOUBLE:
            tok->empty = 8;
            tok->size = 8;
            break;
        case SOCKET_HEAD_UINT_8:
            tok->empty = 1;
            tok->size = 1;
            break;
        case SOCKET_HEAD_UINT_16:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_UINT_32:
            tok->empty = 4;
            tok->size = 4;
            break;
        case SOCKET_HEAD_UINT_64:
            tok->empty = 8;
            tok->size = 8;
            break;
        case SOCKET_HEAD_INT_8:
            tok->empty = 1;
            tok->size = 1;
            break;
        case SOCKET_HEAD_INT_16:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_INT_32:
            tok->empty = 4;
            tok->size = 4;
            break;
        case SOCKET_HEAD_INT_64:
            tok->empty = 8;
            tok->size = 8;
            break;
        case SOCKET_HEAD_FIXEXT_1:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_FIXEXT_2:
            tok->empty = 3;
            tok->size = 3;
            break;
        case SOCKET_HEAD_FIXEXT_4:
            tok->empty = 5;
            tok->size = 5;
            break;
        case SOCKET_HEAD_FIXEXT_8:
            tok->empty = 9;
            tok->size = 9;
            break;
        case SOCKET_HEAD_FIXEXT_16:
            tok->empty = 17;
            tok->size = 17;
            break;
        case SOCKET_HEAD_STR_8:
            tok->empty = 1;
            tok->size = 1;
            break;
        case SOCKET_HEAD_STR_16:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_STR_32:
            tok->empty = 4;
            tok->size = 4;
            break;
        case SOCKET_HEAD_ARRAY_16:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_ARRAY_32:
            tok->empty = 4;
            tok->size = 4;
            break;
        case SOCKET_HEAD_MAP_16:
            tok->empty = 2;
            tok->size = 2;
            break;
        case SOCKET_HEAD_MAP_32:
            tok->empty = 4;
            tok->size = 4;
            break;
        case SOCKET_HEAD_EMPTY:
            tok->empty = 0;
            tok->size = 0;
            break;
    }
 
    tok->content = malloc(sizeof(char)*tok->empty);
    tok->size = 0;
}
static t_Parser_State* init_parser(t_Parser_State* parser, char* text){
    if(parser==NULL){
        parser = malloc(sizeof(t_Parser_State));
    }
    parser->pos = malloc(sizeof(char)*strlen(text));
    strcpy(parser->pos, text);
    parser->tokens = malloc(sizeof(t_Parser_Token)*5);
    parser->tokenCount = 0;
    parser->tokenCount = 5;
    return parser;
}
static t_Response* init_response(t_Response* r){
    return NULL;
}
t_Socket* init_server(t_Socket* server, const char* path, unsigned int ports){
    if(server==NULL){
        server = malloc(sizeof(t_Socket));
    }
    
    server->socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    server->addr.sun_family = AF_UNIX;
    strcpy(server->addr.sun_path, path);
    
    server->path = malloc(sizeof(char)*strlen(path));
    strcpy(server->path, path);

    server->io = malloc(sizeof(void*));
    
    bind(server->socketFd, (struct sockaddr*)&server->addr, sizeof(struct sockaddr_un));
    listen(server->socketFd, ports);
    return server;
}
t_Socket* init_client(t_Socket* client, const char* path){
    if(client==NULL){
        client = malloc(sizeof(t_Socket));
    }
    
    client->socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
    client->addr.sun_family = AF_UNIX;
    strcpy(client->addr.sun_path, path);
    
    client->path = malloc(sizeof(char)*strlen(path));
    strcpy(client->path, path);

    client->io = malloc(sizeof(void*));
    
    connect(client->socketFd, (struct sockaddr*)&client->addr, sizeof(struct sockaddr_un));
    return client;
}


static t_Parser_State* append(t_Parser_State* parser, t_Parser_Token* token){
    if(parser->tokenAllocated <= parser->tokenCount){
        parser->tokens = realloc(parser->tokens, sizeof(t_Parser_Token)*(parser->tokenAllocated+5));
        parser->tokenAllocated = parser->tokenAllocated+5;
    }
    memcpy(&parser->tokens[parser->tokenCount], token, sizeof(t_Parser_Token));
    parser->tokenCount++;
    return parser;
}
static t_Parser_State* parse(t_Parser_State* parser, char next){
    t_Parser_Token currentToken = parser->tokens[parser->tokenCount-1];
    if(currentToken.empty==0){
        t_Parser_Token* nextToken = init_token(NULL, next);
        return parser;
    }
    currentToken.content[currentToken.size-currentToken.empty] = next;
    currentToken.content;
}
static char* recursive_get_next(t_Socket* sock, char* buff, size_t offset){
    t_Parser_State *parser = init_parser(NULL, SOCKET_HEAD_HEADER);
    size_t i = 0;
    while(1){
        buff = realloc(buff, i+1024);
        char* pos = buff+i;
        for(int j=0; j<1024; j++){
            if(*pos==SOCKET_HEAD_FINISH){
                
                return NULL;
            }
            read(sock->socketFd, pos, 1);
            pos++;
        }
        i = i+1024;
    }
}
static t_Response* get_next(t_Socket* sock){
    t_Response* resp = init_response(NULL);
}
static t_Status socket_send(t_Socket* s, void* data){
}



