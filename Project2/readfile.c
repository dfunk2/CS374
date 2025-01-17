#include <stdio.h> 
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char* argv[]) {
    char buffer[2048]= {0};

    int fd = open(argv[1], O_RDONLY); //open returns a file descriptor
    
    if(fd == -1) {
        perror("Unable to open file\n");
        return 1;
    }
        
    int bytes_read = read(fd, buffer, 2048); //read takes a file descriptor and returns bytes read

    write(1, buffer, bytes_read); //writes to screen

    close(fd); //closes a file descriptor

    return 0;
}