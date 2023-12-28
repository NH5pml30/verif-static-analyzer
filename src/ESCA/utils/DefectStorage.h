#ifndef DEFECT_STORAGE_H
#define DEFECT_STORAGE_H

#include <clang/Basic/Diagnostic.h>
#include <def.h>
#include <string>
#include <unordered_set>
#include <vector>

#include <clang/Frontend/TextDiagnosticPrinter.h>

/// @class Синглтон класса в котором сохраняются все найденные утечки ресурсов
class DefectStorage {
private:
  bool IsDuplicate(const std::string &loc) {
    return !defectsLocations.insert(loc).second;
  }

  clang::DiagnosticIDs::Level GetLevel(const std::string &loc,
                                       clang::DiagnosticIDs::Level level) {
    return IsDuplicate(loc) ? clang::DiagnosticIDs::Ignored : level;
  }

public:
  /// @brief Предоставляет единственный экземпляр
  static DefectStorage &Instance();

  bool IsIgnored(clang::SourceLocation loc) const {
    return provider->IsIgnored(loc);
  }

  void SetDiagProvider(DiagProvider *provider) { this->provider = provider; }

  void ClearLocations() {
    if (auto nProblems = defectsLocations.size()) {
      Diag("<report>", "Found %0 problems", clang::DiagnosticIDs::Note)
          << nProblems;
    }
    defectsLocations.clear();
  }

  clang::DiagnosticBuilder
  Diag(clang::SourceLocation loc, llvm::StringRef description,
       clang::DiagnosticIDs::Level level = clang::DiagnosticIDs::Warning) {
    return provider->Diag(loc, description,
                          GetLevel(provider->GetLocString(loc), level));
  }

  clang::DiagnosticBuilder
  Diag(const std::string &loc, llvm::StringRef description,
       clang::DiagnosticIDs::Level level = clang::DiagnosticIDs::Warning) {
    return provider->Diag({}, description, GetLevel(loc, level));
  }

  DefectStorage(const DefectStorage &inst) = delete;

  DefectStorage &operator=(const DefectStorage &rhs) = delete;

private:
  DefectStorage() = default;

  std::unordered_set<std::string> defectsLocations;
  DiagProvider *provider;
};

#endif
