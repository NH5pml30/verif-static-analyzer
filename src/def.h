#ifndef DEF_H
#define DEF_H

#include <llvm/ADT/StringRef.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>

class DiagProvider {
protected:
  const char *CheckName;
  clang::DiagnosticsEngine *DiagEngine;

public:
  DiagProvider(const char *CheckName, clang::DiagnosticsEngine *DiagEngine)
      : CheckName(CheckName), DiagEngine(DiagEngine) {}

  clang::DiagnosticBuilder
  diag(clang::SourceLocation Loc, llvm::StringRef Description,
       unsigned *IDPtr = nullptr,
       clang::DiagnosticIDs::Level Level = clang::DiagnosticIDs::Warning) {
    assert(DiagEngine);
    unsigned ID = DiagEngine->getDiagnosticIDs()->getCustomDiagID(
        Level, (Description + " [" + CheckName + "]").str());
    if (IDPtr)
      *IDPtr = ID;
    return DiagEngine->Report(Loc, ID);
  }

  std::string getLocString(clang::SourceLocation Loc) {
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
  create(clang::ASTContext &AstContext, llvm::StringRef InFile) = 0;
  virtual void finish() = 0;

  virtual ~VerifModuleBase() = default;
};

#endif
