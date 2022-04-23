include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(test_util test.cpp)

target_link_libraries(test_util gtest)