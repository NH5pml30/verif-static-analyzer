#include <iostream>
#include <fstream>
#include <filesystem>
#include <optional>

#include <clang/Frontend/ASTUnit.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/MultiplexConsumer.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <llvm/Support/CommandLine.h>

#include "ESCA/ESCA.h"
#include "GoToLater/GoToLater.h"

static llvm::cl::opt<std::string>
    DefectsFileName("defects-out", llvm::cl::desc("Defects output filename"),
                    llvm::cl::init("-"));

static llvm::cl::opt<std::string>
    IgnoreFilesOpt("ignore-files", llvm::cl::desc("Ignore diagnostics in these files"),
                    llvm::cl::init(""));

llvm::Regex IgnoreFilesRegex;

bool DiagProvider::IsIgnored(clang::SourceLocation Loc) const {
  if (DiagEngine->getSourceManager().isInSystemHeader(Loc) ||
      DiagEngine->getSourceManager().isInSystemMacro(Loc))
    return true;
  if (IgnoreFilesRegex.match(DiagEngine->getSourceManager().getFilename(Loc)))
    return true;
  return false;
}

static std::vector<std::unique_ptr<VerifModuleBase>> Modules;

class VerifAction : public clang::ASTFrontendAction {
  clang::DiagnosticsEngine *Diagnostics;

public:
  VerifAction(clang::DiagnosticsEngine *Diagnostics)
      : Diagnostics(Diagnostics) {}

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &Compiler,
                    llvm::StringRef InFile) override {
    std::vector<std::unique_ptr<clang::ASTConsumer>> Consumers;
    Diagnostics->setSourceManager(&Compiler.getSourceManager());
    for (auto &Module : Modules)
      Consumers.push_back(Module->Create(Compiler.getASTContext(), InFile));
    return std::make_unique<clang::MultiplexConsumer>(std::move(Consumers));
  }
};

class VerifActionFactory : public clang::tooling::FrontendActionFactory {
  clang::DiagnosticsEngine *Diagnostics;

public:
  VerifActionFactory(clang::DiagnosticsEngine *Diagnostics)
      : Diagnostics(Diagnostics) {}

  std::unique_ptr<clang::FrontendAction> create() override {
    return std::make_unique<VerifAction>(Diagnostics);
  }
};

static llvm::cl::OptionCategory VerifToolCategory("verif options");

// Module registration: add modules here
static void RegisterModules(clang::DiagnosticsEngine *DiagEngine) {
  Modules.push_back(std::make_unique<ESCA>(DiagEngine));
  Modules.push_back(std::make_unique<GoToLater>(DiagEngine));
}

int main(int argc, const char *argv[]) {
  auto ExpectedOptParser = clang::tooling::CommonOptionsParser::create(argc, argv, VerifToolCategory);
  if (!ExpectedOptParser) {
    llvm::errs() << ExpectedOptParser.takeError() << "\n";
    return 1;
  }

  auto &OptParser = *ExpectedOptParser;
  clang::IntrusiveRefCntPtr<clang::FileManager> Files =
      new clang::FileManager(clang::FileSystemOptions());
  clang::tooling::ClangTool Tool(
      OptParser.getCompilations(), OptParser.getSourcePathList(),
      std::make_shared<clang::PCHContainerOperations>(),
      llvm::vfs::getRealFileSystem(), Files);

  clang::IntrusiveRefCntPtr<clang::DiagnosticOptions> DiagOpts =
      new clang::DiagnosticOptions;
  IgnoreFilesRegex = llvm::Regex(IgnoreFilesOpt);
  std::optional<llvm::raw_fd_ostream> OS;
  if (DefectsFileName != "-") {
    std::error_code EC;
    OS.emplace(DefectsFileName, EC);
    if (EC) {
      llvm::errs() << EC.message() << "\n";
      return 1;
    }
  }
  clang::TextDiagnosticPrinter DiagnosticPrinter(
      OS.has_value() ? *OS : llvm::outs(), DiagOpts.get());
  auto Diagnostics = clang::CompilerInstance::createDiagnostics(
      DiagOpts.get(), &DiagnosticPrinter, false);
  Tool.setDiagnosticConsumer(&DiagnosticPrinter);

  RegisterModules(Diagnostics.get());

  VerifActionFactory Fac(Diagnostics.get());
  if (Tool.run(&Fac))
    return 1;

  clang::SourceManager SrcMgr(*Diagnostics, *Files);
  Diagnostics->setSourceManager(&SrcMgr);
  clang::LangOptions opts;
  DiagnosticPrinter.BeginSourceFile(opts, nullptr);
  for (auto &Module : Modules)
    Module->Finish();
  Modules.clear();
  DiagnosticPrinter.EndSourceFile();

  return 0;
}
