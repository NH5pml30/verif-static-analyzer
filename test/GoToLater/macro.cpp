// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s

#define BACKJUMP(x)                                                            \
  do {                                                                         \
  back:                                                                        \
    x--;                                                                       \
    if (x)                                                                     \
      goto back;                                                               \
  } while (false);

int main() {
  int x = 10;
  // CHECK: warning: goto back jump
  BACKJUMP(x);
  return 0;
}
