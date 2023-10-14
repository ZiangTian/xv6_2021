#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h" // defines MAXARG


#define BUFSIZE 128

int
main(int argc, char** argv)
{
    sleep(10);
    // obtain the output of other command
    // the output consists of multiple lines. run the cmd after args for each line.
    char buf[BUFSIZE];
    read(0, buf, BUFSIZE);


    // obtain the arguments of itself
    char *xargv[MAXARG];
    int xargc = 0;    // argv[0] is xargs; following xargs is a command

    for(int i = 1; i < argc; i++){
        xargv[xargc] = argv[i];  // xargv[0] is cmd
        xargc += 1;
    }
    char *p = buf;

    // parse the buf
    for(int i = 0; i < BUFSIZE; i++){
        if(buf[i] == '\n'){ // hit the end of a line
            // execute the cmd
            buf[i] = 0; // chop the buf  

            int pid = fork();
            if(pid > 0){
                p = &(buf[i+1]); // advance p by 1
                wait(0);
            }else if (pid == 0)
            {
                          
                xargv[xargc] = p; // add the part to be the last arg
            
                // execute the command
                exec(xargv[0], xargv); // note that the first arg is a path and the second arg should include the cmd
                exit(0);
            }
            else
            {
                fprintf(2, "failed to fork.\n");
                exit(1);
            }

        }
    }
   
    exit(0);
}