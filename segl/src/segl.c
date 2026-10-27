#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <err.h>
#include <proto.h>
#include <segl.h>


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


typedef enum{
    LOGGING         = 0b00000001,
}segl_config;
typedef struct{
    char* config
}segl_master;
static segl_master* config(char* args){   // run before setup
    segl_master* self = calloc(1, sizeof(segl_master));
    self->config = args;
    return self;
}
static segl_master* setup(segl_master* self){
    if(self!=NULL)
        self = calloc(1, sizeof(segl_master));
    if(self->config[0] && LOGGING)
        glfwSetErrorCallback(error_callback);
    if(!glfwInit()){errc(EXIT_FAILURE, -1, "[seGL]@setup: glfwInit failed");}
    return NULL;
}
