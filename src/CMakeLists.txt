add_executable(zap zap/main.cpp zap/segment.cpp zap/coding.cpp)

target_link_libraries(zap folly fmt)

add_executable(bbolt bbolt/main.cpp)

add_executable(duckdb duckdb/main.cpp duckdb/MetaBlockReader.cpp duckdb/serializer.cpp)