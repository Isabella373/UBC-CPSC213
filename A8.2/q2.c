

int foo(int i){
    int j;
    switch(i) {
        case 10: j = a + b; break;
        case 12: j = a - b; break;
        case 14:  if (a>b) {
            j = 1;
        } else {
            j = 0;
        } break;
        case 16: if(a < b) {
            j = 1;
        } else {
            j = 0;
        } break;
        case 18: if (a==b){
            j = 1;
        } else {
            j = 0;
        } break;

        default:
        j = 0;
        break;
    }

    return j;
}

int foo_using_jumptable () {
  static const void* jumpTable[4] = { &&L20, &&L21, &&DEFAULT, &&L23 };
  
  if (i < 10 || i > 18 || i = 11 || i= 13 || i = 15 || i = 17) goto DEFAULT;
  goto *jumpTable [i-10];
L10:
  j = a+b;
  goto CONT;
L12:
  j = a - b;
  goto CONT;
L14:
  if (a>b) {
            j = 1;
        } else {
            j = 0;}
  goto CONT;
L16:
  if(a < b) {
            j = 1;
        } else {
            j = 0;
        } 
  goto CONT;
L18:
  if (a==b){
            j = 1;
        } else {
            j = 0;
        }
  goto CONT;
DEFAULT:
  j = 0;
  goto CONT;
CONT:
  return j;
}

int main (int argc, char** argv) {
  for (i=10; i<=18; i++)
    printf ("%d: %d %d %d\n", i, foo(), foo_using_if(), foo_using_jumptable());
}