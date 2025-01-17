#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
int main () {

    char x[10] = {0};
    char y[10] = {0};
    char z[10] = {0};
    
    read(0, x, 10);
    read(0, y, 10);
    read(0, z, 10);


    printf("1:%s2:%s3:%s", x, y, z);

}