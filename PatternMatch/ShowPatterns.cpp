#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Config/llvm-config.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Compiler.h"

struct ShowPatterns : llvm::PassInfoMixin<ShowPatterns> {

  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionPassManager &FAM) {
    ;
    return llvm::PreservedAnalyses::all();
  }
};

llvm::PassPluginLibraryInfo getShowPatternsPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ShowLoops", LLVM_VERSION_STRING,
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::FunctionPassManager &PM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  return true;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getShowPatternsPluginInfo();
}