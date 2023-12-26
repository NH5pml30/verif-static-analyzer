// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s

int main() {
  int x = 10;
  // CHECK: {{.*}}same-line.cpp:[[# @LINE + 2]]:19: warning: goto back jump
  // CHECK: {{.*}}same-line.cpp:[[# @LINE + 1]]:1: note: label declared here
back: x--; if (x) goto back;
  return 0;
}
