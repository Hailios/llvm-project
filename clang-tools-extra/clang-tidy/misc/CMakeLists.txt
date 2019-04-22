set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyMiscModule
  DefinitionsInHeadersCheck.cpp
  MakeRefCheckCheck.cpp
  MiscTidyModule.cpp
  MisplacedConstCheck.cpp
  MyFirstCheckCheck.cpp
  MySecondCheckCheck.cpp
  NewDeleteOverloadsCheck.cpp
  NonCopyableObjects.cpp
  NonPrivateMemberVariablesInClassesCheck.cpp
  RedundantExpressionCheck.cpp
  StaticAssertCheck.cpp
  ThrowByValueCatchByReferenceCheck.cpp
  UnconventionalAssignOperatorCheck.cpp
  UniqueptrResetReleaseCheck.cpp
  UnusedAliasDeclsCheck.cpp
  UnusedParametersCheck.cpp
  UnusedUsingDeclsCheck.cpp

  LINK_LIBS
  clangAnalysis
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangSerialization
  clangTidy
  clangTidyUtils
  clangTooling
  )
