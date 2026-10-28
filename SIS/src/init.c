#include <kernel.h>
#include <init.h>




int main(){
    proto* p = init_proto(NULL, "init");
    p->free(p);
    return 0;
}
