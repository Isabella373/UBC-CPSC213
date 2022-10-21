#include <stdio.h>


int foo(int i, int a, int b) {
    int r;
    switch (i)
    {
    case 10:
        r = a + b;
        break;

    case 12:
        r = a - b;
        break;

    case 14:
       if (a > b) {
           r = 1;
       } else {
           r = 0;
       }
        break;

    case 16:
        if (a < b) {
            r = 1;
        } else {
            r = 0;
        }
        break;

    case 18:
        if (a == b)
        {
            r = 1;
        }
        else
        {
            r = 0;
        }
        break;

    default:
        r = 0;
        break;
    }

    return r;
}

// int main(int argc, char **argv)
// {
//     for (i = 19; i <= 24; i++)
//         printf("%d: %d %d %d\n", i, foo(), foo_using_if(), foo_using_jumptable());
// }


int main(int argc, char const *argv[])
{
    for (int i = 10; i <= 18; i++) {
        printf("%d\n", foo(i,))
    }
        return 0;
}
