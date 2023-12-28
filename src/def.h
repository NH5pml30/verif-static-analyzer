#ifndef DEF_H
#define DEF_H

#include "clang/Basic/SourceManager.h"
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/Diagnostic.h>
#include <llvm/ADT/StringRef.h>

class DiagProvider {
protected:
  const char *CheckName;
  clang::DiagnosticsEngine *DiagEngine;

public:
  virtual bool IsIgnored(clang::SourceLocation Loc) const;

  DiagProvider(const char *CheckName, clang::DiagnosticsEngine *DiagEngine)
      : CheckName(CheckName), DiagEngine(DiagEngine) {}

  clang::DiagnosticBuilder
  Diag(clang::SourceLocation Loc, llvm::StringRef Description,
       clang::DiagnosticIDs::Level Level = clang::DiagnosticIDs::Warning) {
    assert(DiagEngine);
    if (IsIgnored(Loc))
      Level = clang::DiagnosticIDs::Ignored;
    unsigned ID = DiagEngine->getDiagnosticIDs()->getCustomDiagID(
        Level, (Description + " [" + CheckName + "]").str());
    return DiagEngine->Report(Loc, ID);
  }

  std::string GetLocString(clang::SourceLocation Loc) const {
    return Loc.printToString(DiagEngine->getSourceManager());
  }
};

class VerifModuleBase : public DiagProvider {
protected:
  VerifModuleBase(const char *VerifModuleName,
                  clang::DiagnosticsEngine *DiagEngine)
      : DiagProvider(VerifModuleName, DiagEngine) {}

public:
  virtual std::unique_ptr<clang::ASTConsumer>
  Create(clang::ASTContext &AstContext, llvm::StringRef InFile) = 0;
  virtual void Finish() = 0;

  virtual ~VerifModuleBase() = default;
};

#endif
