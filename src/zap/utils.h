#ifndef ZAP_INCLUDE_UTILS_H_
#define ZAP_INCLUDE_UTILS_H_
#include <bitset>
#include <iostream>

#include "commons.h"

namespace zap {
int32_t readInt32(const char *data_, int off) {
  uint8_t *ptr = reinterpret_cast<uint8_t *>(const_cast<char *>(data_));
  return (int32_t(ptr[off]) << 24) + (int32_t(ptr[off + 1]) << 16) +
         (int32_t(ptr[off + 2]) << 8) + int32_t(ptr[off + 3]);
}

uint32_t readUInt32(const char *data_, int off) {
  uint8_t *ptr = reinterpret_cast<uint8_t *>(const_cast<char *>(data_));
  return (uint32_t(ptr[off]) << 24) + (uint32_t(ptr[off + 1]) << 16) +
         (uint32_t(ptr[off + 2]) << 8) + uint32_t(ptr[off + 3]);
}

uint64_t readUInt64(const char *data_, int off) {
  uint64_t h = readUInt32(data_, off);
  uint64_t l = readUInt32(data_, off + 4);
  return (h << 32) + l;
}

void printBinary(char x) { std::cout << std::bitset<sizeof(x) * 8>(x) << " "; }
} // namespace zap

#endif