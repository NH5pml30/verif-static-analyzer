// RUN: %verif %s | %FileCheck %s

int main() {
// CHECK: Hello, from verif-static-analyzer!
  return 0;
}
