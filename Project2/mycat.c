#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char buffer[2048];

    if(argc == 1) { //no arguments provided
        int fd = open(argv[0], O_RDONLY);
            int bytes_read = read(fd, buffer, 2048);
            write(1, buffer, bytes_read);
            
    } else {
        for (int i = 0; i <= argc; i++) {
            int fd = open(argv[i], O_RDONLY);
                if(fd == -1) {
                    perror("Unable to open file\n");
                    return 1;
                }
            int bytes_read = read(fd, buffer, 2048);
            write(1, buffer, bytes_read);
            
            if(bytes_read == 0) {
                break; //end of file
            }

            close(fd);
        }

    } 
    return 0;
}