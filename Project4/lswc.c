#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
/*Denise Funk
CS 374*/


int main (int argc, char *argv[]) {
    //current directory 
    char* dir = ".";

    //parse command line
    if(argc > 1){
        dir = argv[1];
    }
   
    //create pipe
     int pfd[2];
    if(pipe(pfd) == -1){
        perror("pipe error");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if(pid == 0){
        //child process
        close(pfd[1]);

        if(dup2(pfd[0], STDIN_FILENO) == -1){
            perror("read error\n");
            return EXIT_FAILURE;
        }
        execlp("wc", "wc", "-l", (char *)NULL);
        perror("wc error");
        exit(1);
        close(pfd[0]);

    }else if(pid > 0){
        //parent process
        
        close(pfd[0]);

        if(dup2(pfd[1], STDOUT_FILENO) == -1){
            perror("write error");
            return EXIT_FAILURE;
        }
        execlp("ls", "ls", "-1a", dir, (char*)NULL);
        perror("ls error");

    }
    close(pfd[0]);
    close(pfd[1]);
    wait(NULL);
    return 0;
 
}
