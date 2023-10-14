#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "Usage: sleep time... \n");
        exit(1);
    } else
    {
        int some_time = atoi(argv[1]);
        sleep(some_time);
        exit(0);
    }
}