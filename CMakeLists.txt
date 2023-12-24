cmake_minimum_required(VERSION 3.20)

project(loop-check)

set(CMAKE_CXX_STANDARD 23)

find_package(LLVM REQUIRED CONFIG NO_DEFAULT_PATH HINTS ~/llvm)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
include_directories(${LLVM_INCLUDE_DIRS})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_subdirectory(ShowLoops)
add_subdirectory(PatternMatch)