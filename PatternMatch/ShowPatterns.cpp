#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Config/llvm-config.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/raw_ostream.h"

struct ShowPatterns : llvm::PassInfoMixin<ShowPatterns> {

  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM) {
    using namespace llvm::PatternMatch;
    llvm::outs();
    for (auto &BB : F) {
      for (auto &I : BB) {
        llvm::Value *X, *Y;
        if (match(&I, m_Add(m_Value(X), m_Value(Y)))) {
          // I is an add instruction, and X and Y are its operands
          llvm::outs() << "Found an add instruction: " << I << "\n";
          llvm::outs() << "Operand 1: " << *X << "\n";
          llvm::outs() << "Operand 2: " << *Y << "\n";
        }
      }
    }
    return llvm::PreservedAnalyses::all();
  }
};

llvm::PassPluginLibraryInfo getShowPatternsPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ShowPatterns", LLVM_VERSION_STRING,
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::FunctionPassManager &PM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (Name == "show-patterns") {
                    PM.addPass(ShowPatterns());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getShowPatternsPluginInfo();
}