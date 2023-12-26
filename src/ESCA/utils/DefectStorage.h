#ifndef DEFECT_STORAGE_H
#define DEFECT_STORAGE_H

#include <vector>
#include <string>
#include <set>
#include <clang/Basic/Diagnostic.h>
#include <def.h>

#include <type_traits>
#include <clang/Frontend/TextDiagnosticPrinter.h>

template<typename Tag>
struct public_cast_traits;

template<typename Tag>
struct public_cast {
    static inline typename public_cast_traits<Tag>::type value{};
};

template<typename Tag, typename public_cast_traits<Tag>::type MemPtr>
struct access_t {
    static const inline auto value = public_cast<Tag>::value = MemPtr;
};

template <>
struct public_cast_traits<class CxTag> {
    using type = std::unique_ptr<clang::TextDiagnostic>
                             clang::TextDiagnosticPrinter::*;
};
template struct access_t<CxTag, &clang::TextDiagnosticPrinter::TextDiag>;

/// @class Синглтон класса в котором сохраняются все найденные утечки ресурсов
class DefectStorage
{
private:
    struct DefectBuilder : clang::DiagnosticBuilder {
        unsigned id;
        std::string loc;

        ~DefectBuilder() {
            if (DefectStorage::Instance()
                    .defectsLocations.insert(std::make_pair(id, loc))
                    .second) {
                Emit();
            }
            Clear();
        }
    };

public:
    /// @brief Предоставляет единственный экземпляр
    static DefectStorage &Instance();

    void SetDiagProvider(DiagProvider *provider) {
        this->provider = provider;
    }

    void ClearLocations() {
        if (!defectsLocations.empty()) {
            Diag("<report>", "Found %0 problems", clang::DiagnosticIDs::Note)
                << defectsLocations.size();
        }
        defectsLocations.clear();
    }

    DefectBuilder
    Diag(clang::SourceLocation Loc, llvm::StringRef Description,
         clang::DiagnosticIDs::Level Level = clang::DiagnosticIDs::Warning) {
        unsigned id{};
        auto res = provider->diag(Loc, Description, &id, Level);
        return DefectBuilder{res, id, provider->getLocString(Loc)};
    }

    DefectBuilder
    Diag(const std::string &Loc, llvm::StringRef Description,
         clang::DiagnosticIDs::Level Level = clang::DiagnosticIDs::Warning) {
        unsigned id{};
        auto res = provider->diag({}, Description, &id, Level);
        return DefectBuilder{res, id, Loc};
    }

    DefectStorage( const DefectStorage &inst ) = delete;

    DefectStorage &operator=( const DefectStorage &rhs ) = delete;

private:
    DefectStorage() = default;

    std::set<std::pair<unsigned, std::string>> defectsLocations;
    DiagProvider *provider;
};

#endif
