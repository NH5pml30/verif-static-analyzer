// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s

#define BACKJUMP(x)                                                            \
  do {                                                                         \
    BACKLABEL(x);                                                              \
    if (x)                                                                     \
      goto back;                                                               \
  } while (false);

#define BACKLABEL(x)                                                           \
  back:                                                                        \
  x--;

int main() {
  int x = 10;
  // CHECK-NOT: warning: goto back jump
  BACKJUMP(x);
  return 0;
}
