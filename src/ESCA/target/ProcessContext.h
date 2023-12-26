#include "../FSM/FSM.h"

namespace Target
{
/// @class Контекст для одной функции, хранит автомат операций
class ProcessContext
{
public:
    explicit ProcessContext( const std::string &funName )
            : fsm(std::make_unique<FSM>(funName))
    {
    }

    std::map<std::string, PtrCounter> variables;

    /// @brief Вектор аллоцированных переменных
    std::vector<VersionedVariable> allocatedVars;

    /// @brief Автомат который хранит все операции(состояния) функции
    std::unique_ptr<FSM> fsm;

    FairLeafPredicate fairPred;
    BranchLeafPredicate branchPred;
};
}