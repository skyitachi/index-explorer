project(index-explorer)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_subdirectory(src)