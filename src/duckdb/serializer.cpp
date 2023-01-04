//
// Created by Shiping Yao on 2023/1/4.
//
#include "serializer.h"

namespace explorer {
namespace duckdb {

template <>
std::string Deserializer::Read() {
  uint32_t size = Read<uint32_t>();
  if (size == 0) {
    return std::string();
  }
  auto buffer = std::unique_ptr<data_t[]>(new data_t[size]);
  ReadData(buffer.get(), size);
  return std::string((char *)buffer.get(), size);
}

void Deserializer::ReadStringVector(std::vector<std::string> &list) {
  uint32_t sz = Read<uint32_t>();
  list.resize(sz);
  for (idx_t i = 0; i < sz; i++) {
    list[i] = Read<std::string>();
  }
}

}
}