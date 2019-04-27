//===--- MakeRefCheckCheck.cpp - clang-tidy -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MakeRefCheckCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void MakeRefCheckCheck::registerMatchers(MatchFinder *Finder) {
  // Requires C++.
  if (!getLangOpts().CPlusPlus)
    return;

  Finder->addMatcher(
      callExpr(
          // check the return type of the call and make sure it is either * I or a pointer to a derived class of I
          hasType(
              pointerType(
                  pointee(
                      recordType(
                          hasDeclaration(
                              cxxRecordDecl(
                                  anyOf(
                                      isDerivedFrom("I"),
                                      hasName("I")))))))),
          // check if there already is a call to make_ref which this call feeds in to
          unless(
              ignoringImpCasts(
                  hasParent(
                      callExpr(
                          callee(
                              functionDecl(
                                  matchesName("make_ref"))))))),
          // exclude methods from smart pointer,
          // methods that returns a raw pointer from the smart ptr should not get warnings
          unless(
              cxxMemberCallExpr(
                  callee(
                      memberExpr(
                          hasDeclaration(
                              decl(
                                  cxxMethodDecl(
                                      ofClass(
                                          classTemplateSpecializationDecl(
                                              hasName("B"))))))))))
      ).bind("ExprNotHandledByMakeRef"), this);
}

void MakeRefCheckCheck::check(const MatchFinder::MatchResult &Result) {
  if(const auto *matchedExpr = Result.Nodes.getNodeAs<CallExpr>("ExprNotHandledByMakeRef")) {
    const SourceLocation insertionLocation = matchedExpr->getExprLoc();
    diag(insertionLocation, "missing call to make_ref")
        << FixItHint::CreateInsertion(insertionLocation, "make_ref(")
        << FixItHint::CreateInsertion(matchedExpr->getEndLoc(), ")");
  }
}

} // namespace misc
} // namespace tidy
} // namespace clang
