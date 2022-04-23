add_executable(zap zap/main.cpp zap/segment.cpp)

target_link_libraries(zap folly fmt)