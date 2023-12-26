#ifndef ESCA_H
#define ESCA_H

#include <def.h>
#include <memory>

class ESCA : public VerifModuleBase {
public:
  ESCA(clang::DiagnosticsEngine *DiagEngine);
  ESCA(const ESCA &) = delete;
  ESCA(ESCA &&) = delete;
  ESCA &operator=(const ESCA &) = delete;
  ESCA &operator=(ESCA &&) = delete;

  std::unique_ptr<clang::ASTConsumer> Create(clang::ASTContext &AstContext,
                                             llvm::StringRef InFile) override;
  void Finish() override;

  ~ESCA();
};

#endif
