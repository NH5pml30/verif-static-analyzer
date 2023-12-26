#ifndef GOTOLATER_H
#define GOTOLATER_H

#include <def.h>
#include <memory>

class GoToLater : public VerifModuleBase {
public:
  GoToLater(clang::DiagnosticsEngine *DiagEngine);

  std::unique_ptr<clang::ASTConsumer> Create(clang::ASTContext &AstContext,
                                             llvm::StringRef InFile) override;
  void Finish() override;

  ~GoToLater();
};

#endif
