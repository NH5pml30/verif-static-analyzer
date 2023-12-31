
#include "Statements.h"

namespace Target
{

void CompoundStatement::AddState( Statement *st )
{
    statements.push_back(st);
}

const std::vector<Statement *> &CompoundStatement::GetStates() const
{
    return statements;
}

void CompoundStatement::SetOptions( const CompoundStatement::IsInOptions &otherOptions )
{
    op = otherOptions;
}

CompoundStatement::IsInOptions CompoundStatement::GetOptions() const
{
    return op;
}

VarAssigmentFromFooStatement::VarAssigmentFromFooStatement( const std::string &varName,
                                                            const std::string &fooName,
                                                            const DefectLocation &loc,
                                                            bool isDecl )
        : isDecl(isDecl), varName(varName), fooName(fooName), loc(loc)
{
}


VarAssigmentFromPointerStatement::VarAssigmentFromPointerStatement( const std::string &varName,
                                                                    const std::string &rhsName,
                                                                    const DefectLocation &loc,
                                                                    bool isDecl )
        : varName(varName), loc(loc), rhsName(rhsName), isDecl(isDecl)
{
}


VarAssigmentNewStatement::VarAssigmentNewStatement( const std::string &varName, bool isArray,
                                                    const DefectLocation &loc, bool isDecl )
        : varName(varName), loc(loc), isArray(isArray), isDecl(isDecl)
{
}


DeleteStatement::DeleteStatement( const std::string &name, bool isArray )
        : name(name), isArray(isArray)
{
}


IfStatement::IfStatement( CompoundStatement *thenSt, CompoundStatement *elseSt, const std::string &condStr,
                          const std::string &elseStr )
        : thenSt(thenSt), elseSt(elseSt), condStr(condStr), elseStr(elseStr)
{
}


ReturnStatement::ReturnStatement( const std::string &returnVarName )
        : returnVarName(returnVarName)
{
}

}
