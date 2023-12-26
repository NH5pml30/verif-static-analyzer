#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>

#include "ESCA.h"
#include "AST/ESCAASTConsumer.h"
#include "utils/common.h"
#include "target/AnalyzeProcess.h"
#include "utils/DefectStorage.h"

#include <llvm/Support/CommandLine.h>

ESCA::ESCA(clang::DiagnosticsEngine *DiagEngine)
    : VerifModuleBase("esca", DiagEngine) {
  DefectStorage::Instance().SetDiagProvider(this);
}

std::unique_ptr<clang::ASTConsumer> ESCA::Create(clang::ASTContext &AstContext,
                                                 llvm::StringRef InFile) {
  if (!CommonStorage::Instance().AddAnalyzeFile(InFile.str())) {
    return {};
  }

  return std::make_unique<ESCAASTConsumer>();
}

void ESCA::Finish() {
  AnalyzeProcess a;
  a.StartAnalyze();

  std::cout << "Working time: " << clock() / CLOCKS_PER_SEC << " sec"
            << std::endl;
}

ESCA::~ESCA() { DefectStorage::Instance().ClearLocations(); }
