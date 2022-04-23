#include <string>
#include <sys/mman.h>

#include <fmt/format.h>

struct Footer {
  uint64_t D;   // Number of Docs
  uint64_t SF;  // stored index offset
  uint64_t F;   // field index offset
  uint64_t DOC; // doc value offset
  uint32_t CF;  // chunk offset
  uint32_t Ver; // version
  uint32_t CC;  // crc32

public:
  const std::string describe() const {
    return fmt::format("number_of_docs={}, stored_index_offset={}, "
                       "field_index_offset={}, doc_value_offset={}, "
                       "chunk_offset={}, version={}, crc={}",
                       D, SF, F, DOC, CF, Ver, CC);
  }
};

class Segment {

public:
  bool Open(const std::string &path);
  const Footer &parseFooter();

  ~Segment() {
    if (is_opened_) {
      munmap(data_, sz_);
    }
  }

private:
  bool is_opened_;
  size_t sz_;
  char *data_ = nullptr;
  Footer footer_;
};