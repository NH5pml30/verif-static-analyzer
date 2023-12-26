#include "GoToLater.h"
#include "GoToLaterASTConsumer.h"

GoToLater::GoToLater(clang::DiagnosticsEngine *DiagEngine)
    : VerifModuleBase("goto-later", DiagEngine) {}

std::unique_ptr<clang::ASTConsumer>
GoToLater::Create(clang::ASTContext &AstContext, llvm::StringRef InFile) {
  return std::make_unique<GoToLaterASTConsumer>(this, AstContext);
}

void GoToLater::Finish() {}

GoToLater::~GoToLater() = default;
