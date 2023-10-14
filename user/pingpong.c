#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
    ''ping-pong'' a byte between two processes over a pair of pipes, 
    one for each direction. The parent should send a byte to the child; 
    the child should print "<pid>: received ping", 
    where <pid> is its process ID, 
    write the byte on the pipe to the parent, and exit; 
    the parent should read the byte from the child, print "<pid>: 
    received pong", and exit.
*/

int
main(int argc, char **argv)
{
    int p[2];
    char buf[2];
    char *send = "a", *rec = "b";

    pipe(p);
    int pid = fork(); // return zero to a child process
                      // return child's pid to parent process

    if(pid == 0){
        
        // child process

        // parent sends a byte to child
        if(read(p[0], buf, 1) != 1){  // did not read 1 byte
            fprintf(2, "child failed to read byte from parent.\n");
            exit(1);
        };
        close(p[0]);

        printf("%d: received ping\n",getpid());

        if(write(p[1], rec, 1) != 1){ // did not write 1 byte
            fprintf(2, "child failed to send byte to parent.");
            exit(1);
        };

        close(p[1]); // close the fd to the write end of the pipe so that read in parent process can end normally.

        exit(0);

    } else if(pid > 0){

        // parent process
        
        if(write(p[1], send, 1) !=1){ // did not write 1 byte
            fprintf(2, "parent failed to send byte to child.\n");
            exit(1);
        };

        close(p[1]); // so that child process can go on to write. otherwise the read in child would be left waiting

        wait(0);
        
        if(read(p[0], buf, 1) != 1) { // did not read 1 byte
            fprintf(2, "parent failed to read byte back from child.\n");
            exit(1);
        };
        close(p[0]);

        printf("%d: received pong\n", getpid());

        exit(0);
    }
    else exit(1);


}