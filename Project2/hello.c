#include <stdio.h>

int main() {
    int x = 12;
    float y = 3.14;
    char* s = "Hello, World!";
    

    printf("x: %d, y: %f\n%s\n", x, y, s);

    int i = 0;
    while(i <= 4) {
        int x = i * 5;
        printf("%d x 5 = %d\n", i, x);
        i++;
    }
    
    
    return 0;
}
