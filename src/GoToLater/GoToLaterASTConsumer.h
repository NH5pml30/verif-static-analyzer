#ifndef GOTOLATERCONSUMER_H
#define GOTOLATERCONSUMER_H

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <def.h>

class GoToLaterASTConsumer : public clang::ASTConsumer {
public:
  class GoToLaterASTVisitor
      : public clang::RecursiveASTVisitor<GoToLaterASTVisitor> {
  public:
    DiagProvider *Provider;
    clang::ASTContext &Context;

    GoToLaterASTVisitor(DiagProvider *Provider, clang::ASTContext &Context)
        : Provider(Provider), Context(Context) {}

    bool VisitGotoStmt(clang::GotoStmt *Goto) {
      if (!CheckGoto(Goto))
        ReportGoto(Goto, Goto->getLabel());
      return true;
    }

    void ReportGoto(const clang::GotoStmt *Goto, const clang::LabelDecl *Lbl) {
      Provider->Diag(Goto->getGotoLoc(), "goto back jump");
      Provider->Diag(Lbl->getLocation(), "label declared here",
                    clang::DiagnosticIDs::Note);
    }

    bool CheckGoto(const clang::GotoStmt *Goto) {
      return Goto->getLabel()->getLocation() > Goto->getGotoLoc();
    }
  };

private:
  GoToLaterASTVisitor Visitor;

public:
  GoToLaterASTConsumer(DiagProvider *Provider, clang::ASTContext &Context)
      : Visitor(Provider, Context) {}

  bool HandleTopLevelDecl(clang::DeclGroupRef DR) override {
    for (auto It : DR)
      Visitor.TraverseDecl(It);
    return true;
  }
};

#endif
