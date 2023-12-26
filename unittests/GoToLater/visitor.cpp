#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include <GoToLater/GoToLaterASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/ASTUnit.h>
#include <clang/Tooling/Tooling.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <gtest/gtest.h>

namespace {
constexpr const char *GOTO_ID = "goto";
using namespace clang::ast_matchers;
class FindGotoCallback : public MatchFinder::MatchCallback {
public:
  const clang::GotoStmt *Goto{};

  virtual void
  run(const MatchFinder::MatchResult &Result) final {
    if (auto *GotoRes = Result.Nodes.getNodeAs<clang::GotoStmt>(GOTO_ID))
      Goto = GotoRes;
  }
};
}

TEST(GoToLaterASTVisitor, BasicBackCheck) {
  auto AST = clang::tooling::buildASTFromCode(R"(
    void f(int x) {
    back:
      goto back;
    }
  )");
  GoToLaterASTConsumer::GoToLaterASTVisitor Vis(nullptr, AST->getASTContext());
  FindGotoCallback C;

  MatchFinder Finder;
  Finder.addMatcher(gotoStmt().bind(GOTO_ID), &C);
  Finder.matchAST(AST->getASTContext());

  EXPECT_NE(C.Goto, nullptr);
  EXPECT_FALSE(Vis.CheckGoto(C.Goto));
}

TEST(GoToLaterASTVisitor, BasicForwardCheck) {
  auto AST = clang::tooling::buildASTFromCode(R"(
    void f(int x) {
      goto back;
    back:
      x++;
    }
  )");
  GoToLaterASTConsumer::GoToLaterASTVisitor Vis(nullptr, AST->getASTContext());
  FindGotoCallback C;

  MatchFinder Finder;
  Finder.addMatcher(gotoStmt().bind(GOTO_ID), &C);
  Finder.matchAST(AST->getASTContext());

  EXPECT_NE(C.Goto, nullptr);
  EXPECT_TRUE(Vis.CheckGoto(C.Goto));
}
