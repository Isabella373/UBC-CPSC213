#include <stdio.h>

int x[8] = {1, 2, 3, -1, -2, 0, 184, 340057058};
int y[8] = {0, 0, 0, 0, 0, 0, 0, 0};


int f(int b) {
    int a = 0;
    while (b) {
        if ((b & 0x80000000) == 0) {
            b = b << 1;
        } else {
            a++;
        }
    }
    return b;
}

int main(int argc, char const *argv[])
{
    int i = 8;
    
    while (i) {
        i--;
        y[i] = f(x[i]);
    }

    for (int i = 0; i < 8; i++) {
        printf("%d\n", x[i]);
    }

    for (int i = 0; i < 8; i++)
    {
        printf("%d\n", y[i]);
    }
}
