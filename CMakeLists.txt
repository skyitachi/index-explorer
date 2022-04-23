project(index-explorer)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_subdirectory(src)

# find_program(CLANG_TIDY_BIN clang-tidy /usr/local/bin/clang-tidy)
# find_program(RUN_CLANG_TIDY_BIN /usr/local/Cellar/llvm/13.0.0_2/share/clang/run-clang-tidy.py)
#   list(APPEND RUN_CLANG_TIDY_BIN_ARGS
# 	   -clang-tidy-binary ${CLANG_TIDY_BIN}
# 	   -header-filter="\".*\\b(src)\""
# 	   -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor,-cppcoreguidelines-*)

# add_custom_target(tidy
# 				COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
# 				COMMENT "running clang tidy")