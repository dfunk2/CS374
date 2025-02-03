#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
/*Denise Funk 
CS 374
 */

int main(int argc, char *argv[]){
    //user error message
    if(argc < 3){
        printf("usage: redirout filename command [arg1 [agr2 ...] ]\n");
        return 1;
    }
    //code to create a file
    int fd;
    if((fd = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0600)) == -1){
        perror("unable to open file\n");
        return 2;
    }

    //child process that saves output in a new file
    if(dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2 error:");
        return 3;
    }
    //command line argument
    
    if(execvp(argv[2], &argv[2]) == -1){
        perror("exevp error:");
        return 4;
    }
    

return 0;
}