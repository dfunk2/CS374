#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

//Unable to get (ls | ./yell) to work but works on text files.

int main(int argc, char *argv[]) {
    char buffer[2048];
    char uppercase[2048];

        for (int i = 1; i <= argc; i++) {
            printf("attemping to open file: %s\n", argv[i]);
            int fd = open(argv[i], O_RDONLY);
            if(fd == -1) {
                perror("Unable to open file\n");
                return 1;
            }
            ssize_t bytes_read;
            
           while((bytes_read = read(fd, buffer, 2048)) >0) {
                for(int i = 0; i < bytes_read; i++) {
                    uppercase[i] = toupper(buffer[i]);
                }
                write(1, uppercase, bytes_read);
            }
    close(fd);
        }//outer for loop 
    return 0;
}