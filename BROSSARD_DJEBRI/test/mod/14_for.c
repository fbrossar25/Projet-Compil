#include <stdio.h>
#define printf(X) fprintf(stdout, "%s", (X))
#define printi(X) fprintf(stdout, "%d", (X))

// Result:
// 42

int main() {
  int a;
  int i;
  a = 0;
  for (i = 0; i < 21; i = i + 1) {
    a = a + 3;
    a = a - 1;
  }
  printi(a);
  printf("\n");
  return 0;
}
