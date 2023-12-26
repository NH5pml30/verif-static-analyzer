#ifndef ESCAASTConsumer_h
#define ESCAASTConsumer_h

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclGroup.h>

#include "ESCAASTVisitor.h"
#include "../utils/common.h"

class ESCAASTConsumer : public clang::ASTConsumer
{
public:
    /// @brief Переопределяемая функция для прохода по AST дереву нашим AST visitor
    bool HandleTopLevelDecl( clang::DeclGroupRef DR ) override
    {
        std::string loc;
        for( auto it : DR )
        {
            loc = it->getLocation().printToString(it->getASTContext().getSourceManager());
//            std::cout << loc << std::endl;
            auto colonId = loc.find(':');
            if( colonId != std::string::npos )
            {
                loc = loc.substr(0, colonId);
            }
            if( CommonStorage::Instance().InIncludeDirs(loc))
            {
                return true;
            }
            CommonStorage::Instance().AddAnalyzeFile(loc);
//            std::cout << loc << std::endl;

//            if( loc.find(Options::Instance().analyzeFile) == std::string::npos )
//            {
//                return true;
//            }

            // Traverse the declaration using our AST visitor.
            visitor.TraverseDecl(it);

        }

        return true;
    }

private:
    ESCAASTVisitor visitor;
};

#endif
