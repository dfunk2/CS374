/*Denise Funk
CS 374
1/23/25*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void RunCommand(char *tokens[]);

int main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    char *tokens[128] = {0};
    char s[2048];

while(1) {
    //shows user prompt
    printf("prompt>");
    fflush(stdout);
    fgets(s, sizeof(s), stdin);
    char *token;
    int i = 0;
    //initiate another prompt if user presses the "enter" key
        if(s[0] == '\n') {
            continue;
        }
    //parse user input
        if((token = strtok(s, " \t\n\r")) != NULL){
            do{
                tokens[i] = token;
                i++;

            }while((token = strtok(NULL, " \t\n\r")) != NULL);
            tokens[i] = NULL;
        }
    //change directories 
        if(strcmp(tokens[0], "cd") == 0) {
            if((tokens[1]) != NULL) {
                if(chdir(tokens[1]) == -1) {
                    perror("change directory error");
                    exit(1);
                }
            } 
        } 
    //exit the program if user types exit
       if(strcmp(tokens[0], "exit") == 0) {
            exit(0);
        }
    //fork a child process
        else{
            RunCommand(tokens);
        }
     }
return 0;
}

void RunCommand(char *tokens[]) {
    pid_t pid = fork();

    if(pid == 0) { //this is the child process. Execvp replaces the shell the child process is running with the program the user requested. 
        puts(tokens[0]);
        execvp(tokens[0], tokens);
        //exit(0);
        //Error with execvp
        perror("execvp");
        exit(1);
    } else if (pid > 0) { //this is the parent process. It waits for the child process to complete
        //puts("Parent waiting for child to complete.\n");
        wait(NULL);
        //puts("Child process is complete.\n");
    } else {
        perror("error, fork failed");
        exit(1);
    }
}