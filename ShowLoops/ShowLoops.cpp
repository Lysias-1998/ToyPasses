#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct ShowLoops : PassInfoMixin<ShowLoops> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    // Get the LoopInfo object for the function
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);

    SmallVector<std::pair<const BasicBlock *, const BasicBlock *> > Result;
    FindFunctionBackedges(F, Result);
    for (auto [Frm, To] : Result) {
      outs() << "***Backedge from ***";
      Frm->print(outs());
      outs() << "@@@ to @@@";
      To->print(outs());
      outs() << "\n";
    }

    // Iterate over the top-level loops in the function
    for (Loop *L : LI) {
      // Print the loop information to the standard output
      outs() << "Loop at depth " << L->getLoopDepth() << ":\n";
      L->print(outs());
      outs() << "\n";
    }

    // Return all analyses as preserved
    return PreservedAnalyses::all();
  }
};

} // namespace

/* New PM Registration */
llvm::PassPluginLibraryInfo getShowLoopsPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ShowLoops", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            // PB.registerVectorizerStartEPCallback(
            //     [](llvm::FunctionPassManager &PM, OptimizationLevel Level) {
            //       PM.addPass(ShowLoops());
            //     });
            errs() << "llvm::getShowLoopsPluginInfo\n";
            PB.registerPipelineParsingCallback(
                [](StringRef Name, llvm::FunctionPassManager &PM,
                   ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (Name == "show-loops") {
                    PM.addPass(ShowLoops());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getShowLoopsPluginInfo();
}
