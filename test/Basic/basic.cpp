// RUN: %verif %s 2>&1 | %FileCheck %s

int main() {
// CHECK: TranslationUnitDecl
// CHECK: Hello, from verif-static-analyzer!
  return 0;
}
