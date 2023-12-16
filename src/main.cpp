#include <clang/Frontend/ASTUnit.h>
#include <clang/Frontend/CompilerInstance.h>
#include <iostream>

int main(int argc, const char *argv[]) {
  clang::ASTUnit *au = clang::ASTUnit::LoadFromCommandLine(
      &argv[0], &argv[argc], std::make_shared<clang::PCHContainerOperations>(),
      clang::CompilerInstance::createDiagnostics(new clang::DiagnosticOptions),
      "");

  au->getASTContext().getTranslationUnitDecl()->dump();

  std::cout << "Hello, from verif-static-analyzer!\n";
  return 0;
}
