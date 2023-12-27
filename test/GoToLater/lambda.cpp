// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s

int main() {
  auto Lambda = [](int x) {
    // CHECK-DAG: {{.*}}lambda.cpp:[[# @LINE + 1]]:5: note: label declared here
    label:
      x--;
      if (x)
        // CHECK-DAG: {{.*}}lambda.cpp:[[# @LINE + 1]]:9: warning: goto back jump
        goto label;
  };
  Lambda(10);
  return 0;
}
