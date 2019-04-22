//===--- MyFirstCheckCheck.cpp - clang-tidy -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MyFirstCheckCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void MyFirstCheckCheck::registerMatchers(MatchFinder *Finder) {
  auto nonAwesomeFunction = functionDecl(unless(matchesName("^::awesome_")));

  Finder->addMatcher(nonAwesomeFunction.bind("addAwesomePrefix"), this);
  Finder->addMatcher(callExpr(callee(nonAwesomeFunction)).bind("addAwesomePrefix"), this);
}

void MyFirstCheckCheck::check(const MatchFinder::MatchResult &Result) {
  SourceLocation insertionLocation;
  if (const auto *matchedDecl = Result.Nodes.getNodeAs<NamedDecl>("addAwesomePrefix")) {
    insertionLocation = matchedDecl->getLocation();
  } else if (const auto *matchedExpr = Result.Nodes.getNodeAs<Expr>("addAwesomePrefix")) {
    insertionLocation = matchedExpr->getExprLoc();
  }
  diag(insertionLocation, "code is insufficiently awesome")
      << FixItHint::CreateInsertion(insertionLocation, "awesome_");
}

} // namespace misc
} // namespace tidy
} // namespace clang
