#ifndef ESCA_H
#define ESCA_H

#include <memory>
#include <def.h>

class ESCA : public VerifModuleBase {
public:
  ESCA(clang::DiagnosticsEngine *DiagEngine);
  ESCA(const ESCA &) = delete;
  ESCA(ESCA &&) = delete;
  ESCA &operator=(const ESCA &) = delete;
  ESCA &operator=(ESCA &&) = delete;

  std::unique_ptr<clang::ASTConsumer> create(clang::ASTContext &AstContext,
                                             llvm::StringRef InFile) override;
  void finish() override;

  ~ESCA();
};

#endif
