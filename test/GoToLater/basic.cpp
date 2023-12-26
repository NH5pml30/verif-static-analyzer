// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s

int main() {
  int x = 10;
  // CHECK-DAG: {{.*}}basic.cpp:[[# @LINE + 1]]:1: note: label declared here
back:
  x--;
  if (x)
    // CHECK-DAG: {{.*}}basic.cpp:[[# @LINE + 1]]:5: warning: goto back jump
    goto back;

  return 0;
}
