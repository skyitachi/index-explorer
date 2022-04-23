#ifndef ZAP_INCLUDE_CODING_H_
#define ZAP_INCLUDE_CODING_H_

#include "slice.h"
#include <stdint.h>

namespace zap {

// void PutVarint64(std::string* dst, uint64_t value);
// Standard Get... routines parse a value from the beginning of a Slice
// and advance the slice past the parsed value.
bool GetVarint32(Slice *input, uint32_t *value);
bool GetVarint64(Slice *input, uint64_t *value);

// Pointer-based variants of GetVarint...  These either store a value
// in *v and return a pointer just past the parsed value, or return
// nullptr on error.  These routines only look at bytes in the range
// [p..limit-1]
const char *GetVarint64Ptr(const char *p, const char *limit, uint64_t *v);

const char *GetVarint32PtrFallback(const char *p, const char *limit,
                                   uint32_t *value);

inline const char *GetVarint32Ptr(const char *p, const char *limit,
                                  uint32_t *value) {
  if (p < limit) {
    uint32_t result = *(reinterpret_cast<const uint8_t *>(p));
    if ((result & 128) == 0) {
      *value = result;
      return p + 1;
    }
  }
  return GetVarint32PtrFallback(p, limit, value);
}

char *EncodeVarint64(char *dst, uint64_t v);

} // namespace zap

#endif // ZAP