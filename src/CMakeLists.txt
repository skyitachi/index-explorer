add_executable(zap zap/main.cpp zap/segment.cpp zap/coding.cpp)

target_link_libraries(zap folly fmt)