#include <stdio.h>

int array[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int *x = array;

int add(int a, int b) {
    x[b] = a + x[b];
}

int foo() {
    int a = 1;
    int b = 2;

    add(3, 4);
    add(a, b);
}

int main(int argc, char const *argv[])
{
    foo();
    for (int i = 0; i < 10; i++) {
        printf("%d\n", array[i]);
    }
    return 0;
}
