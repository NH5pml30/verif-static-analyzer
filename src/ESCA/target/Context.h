#include "Function.h"

namespace Target
{

/// @brief Синглтон структура в которой сохраняется информация о всех состояниях анализируемой программы
class Context
{
public:
    Context( Context const & ) = delete;

    Context &operator=( Context const & ) = delete;

    /// @brief Возвращает единственный экземпляр контекста
    static Context &Instance();

    void ResetFunction();

    /// @brief Текущая анализируемая функция
    Function *curFunction;

    /// @brief Добавляем новую анализируемую функцию
    void AddFunction( const std::string &name );

    /// @brief Метод создает составное состояние и добавляет его на вершину стэка
    CompoundStatement *CreateCompoundStatement();

    /// @brief Метод удаляет последнее составное состояние со стека
    void PopCompound();

    /// @brief Метод добавляет к последнему составному состоянию в стеке вложеное состояние
    void AddToLast( Statement *s );

    /// @brief Метод добавляет к контексту имя функции которая может освобождать ресурсы
    void AddFreeFunction( const std::string &function );

    /// @brief Метод проверяет может ли функция освобождать ресурсы
    bool IsFreeFunction( const std::string &function );

    /// @brief Метод возвращает указатель на мап со всеми функциями
    std::map<std::string, Target::Function *> *GetAllFunction();

    /// @brief Метод создает условное состояние и добавляет его к последнему Compound
    ///        и также добавляет к compoundStatementsStack  then-стэйтмент
    /// @param hasElse - если ли ветка else у условного перехода
    bool CreateIfStatement( bool hasElse, const std::string &cond, const std::string &elseCond );

    /// @brief Метод убирает с compoundStatementsStack then-стэйтмент
    ///        и если есть else добавляет его стэйтмент
    void SwitchToElse();

    void CreateThrow( const std::string &exceptionName );

    bool CreateTryStatement();

    void CreateCatchStatement();

    inline const std::string &GetException() const
    {
        return exceptionName;
    }

    inline bool CatchException() const
    {
        return !exceptionName.empty();
    }

    std::map<std::string, std::string> throwsFunctions;

private:
    /// Конструктор
    Context();

    std::string exceptionName;

    /// @brief Стек с составными состояниями
    std::vector<CompoundStatement *> compoundStatementsStack;

    /// @brief Все фукнции, сохраненные для анализа
    std::map<std::string, Target::Function *> allFunctions;

    /// @brief Фукнции, которые могут освобождать ресурсы
    std::set<std::string> freeFunctions;

};
}
