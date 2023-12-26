// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s

#define FORWARDJUMP(x)                                                         \
  do {                                                                         \
    if (x)                                                                     \
      goto forward;                                                            \
    x--;                                                                       \
  forward:                                                                     \
    x++;                                                                       \
  } while (false);

int main() {
  int x = 10;
  // CHECK-NOT: warning: goto back jump
  FORWARDJUMP(x);
  return 0;
}
