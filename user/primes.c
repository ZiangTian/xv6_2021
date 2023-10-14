#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void prime(int candidate, int* p){
    // p is from the left neighbor
    // candidate is the number that the left neighbor has passed here. 
    // The first candidate becomes blueprint.

    close(p[1]); // close write end
    
    int new_p[2];
    int buf[1];
    int new_candidate;

    pipe(new_p);
    

    int pid = fork();
    int flag = 0;
    if(pid > 0){
        close(new_p[0]); // close read end of the new pipe
        // the parent process reads the candidates and sends the new candidates
        while(read(p[0], buf, sizeof(int)) > 0){
            
            int n = buf[0];
            if(n > 35){
                break;
            }
            if(n % candidate != 0){
                if(flag == 0) {
                    new_candidate = n;
                    printf("prime %d\n", new_candidate);
                    write(new_p[1], &new_candidate, sizeof(int));
                    flag = 1;
                }
                else write(new_p[1], &n, sizeof(int));
            } 
        }
        close(new_p[1]);
        close(p[0]);
        exit(0);
    }
    else if(pid == 0){
        read(new_p[0], &new_candidate, sizeof(int));
        close(p[0]);
        prime(new_candidate, new_p);
        exit(0);
    }
    else {
        fprintf(2, "fork failed.\n");
        exit(1);
    }
    
}


int
main(int argc, char **argv)
{
   int fd[2];
   pipe(fd);

   if(fork() > 0){

    close(fd[0]); 
    printf("prime: 2\n");

    for(int n = 3; n <= 35; n++){
        write(fd[1], &n, sizeof(int));
    }
    close(fd[1]);

    wait(0);
    exit(0);
   }
   else{
    prime(2, fd);
    exit(0);
   }

    
}