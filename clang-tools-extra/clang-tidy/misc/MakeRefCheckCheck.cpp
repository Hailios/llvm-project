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

  auto make_ref_fun = functionDecl(matchesName("make_ref"));

  Finder->addMatcher(callExpr(
      // check the return type of the call and make sure it is either * I or a pointer to a derived class of I
      hasType(
          pointerType(
              pointee(
                  recordType(
                      hasDeclaration(
                          cxxRecordDecl(
                              anyOf(
                                  isDerivedFrom("I"),
                                  hasName("I")
                              )
                          )
                      )
                  )
              )
          )
      ),
      // check if there already is a call to make_ref which this call feeds in to
      unless(
          ignoringImpCasts(
              hasParent(
                  callExpr(
                      callee(make_ref_fun)
                  )
              )
          )
      )
  ).bind("ExprNotHandledByMakeRef"), this);

  // clang-query friendly version of the above
//  callExpr(unless(ignoringImpCasts(hasParent(callExpr(callee(functionDecl(matchesName("make_ref"))))))),hasType(pointerType(pointee(recordType(hasDeclaration(cxxRecordDecl(anyOf(isDerivedFrom("I"),hasName("I")))))))))
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
