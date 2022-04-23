project(index-explorer)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_subdirectory(src)

# clang format
set(INDEXER_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(INDEXER_CLANG_SEARCH_PATH "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin"
        "/usr/local/Cellar/llvm/13.0.0_2/bin")

find_program(CLANG_FORMAT_BIN
        NAMES clang-format clang-format-8
        HINTS ${INDEXER_CLANG_SEARCH_PATH})
if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(WARNING "indexer/main couldn't find clang-format.")
else()
    message(STATUS "indexer/main found clang-format at ${CLANG_FORMAT_BIN}")
endif()


string(CONCAT INDEXER_FORMAT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

# runs clang format and updates files in place.
add_custom_target(format ${INDEXER_BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${INDEXER_BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${INDEXER_FORMAT_DIRS}
        --fix
        --quiet
        )

add_custom_target(check-format ${INDEXER_BUILD_SUPPORT_DIR}/run_clang_format.py
	${CLANG_FORMAT_BIN}
	${INDEXER_BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
	--source_dirs
	${INDEXER_FORMAT_DIRS}
	--quiet
	)

# clang-tidy
find_program(CLANG_TIDY_BIN
        NAMES clang-tidy clang-tidy-8
        HINTS ${INDEXER_CLANG_SEARCH_PATH})
if ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(WARNING "Indexer/main couldn't find clang-tidy.")
else()
    # Output compile_commands.json
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    message(STATUS "Indexer/main found clang-tidy at ${CLANG_TIDY_BIN}")
endif()


add_custom_target(check-clang-tidy
	${INDEXER_BUILD_SUPPORT_DIR}/run-clang-tidy                     # run LLVM's clang-tidy script
	-clang-tidy-binary ${CLANG_TIDY_BIN}                              # using our clang-tidy binary
	-p ${CMAKE_BINARY_DIR}                                            # using cmake's generated compile commands
	)

# test
find_package(gtest REQUIRED)

add_subdirectory(test)