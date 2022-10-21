#include <stdio.h>
int i, a, b, r;

int foo(int i, int a, int b){
    switch(i) {
        case 10: r = a + b; break;
        case 12: r = a - b; break;
        case 14:  if (a > b) {
            r = 1;
        } else {
            r = 0;
        } break;
        case 16: if(a < b) {
            r = 1;
        } else {
            r = 0;
        } break;
        case 18: if (a == b){
            r = 1;
        } else {
            r = 0;
        } break;

        default:
        r = 0;
        break;
    }

    return r;
}

//int foo_using_jumptable (int i) {
//  static const void* jumpTable[6] = { &&L10, &&L12, &&DEFAULT, &&L14, &&16, &&18 };
//
//  if (i < 10 || i > 18 || i = 11 || i= 13 || i = 15 || i = 17) goto DEFAULT;
//  goto *jumpTable [i-10];
//L10:
//  r = a+b;
//  goto CONT;
//L12:
//  r = a - b;
//  goto CONT;
//L14:
//  if (a>b) {
//            r = 1;
//        } else {
//            r = 0;}
//  goto CONT;
//L16:
//  if(a < b) {
//            r = 1;
//        } else {
//            r = 0;
//        }
//  goto CONT;
//L18:
//  if (a==b){
//            r = 1;
//        } else {
//            r = 0;
//        }
//  goto CONT;
//DEFAULT:
//  r = 0;
//  goto CONT;
//CONT:
//  return r;
//}

int main (int argc, char** argv) {
  for (i=10; i<=18; i++)
    printf ("%d: %d \n", i, foo(i, a, b));
}
