//
// Created by Shiping Yao on 2023/1/4.
//

#ifndef INDEX_EXPLORER_STORAGE_INFO_H
#define INDEX_EXPLORER_STORAGE_INFO_H

namespace explorer {
using data_t = uint8_t;
using data_ptr_t = data_t*;
using idx_t = uint64_t;
using block_id_t = int64_t;

class IOException: std::exception {
public:
  explicit IOException(const std::string& r) {
    what_ = r;
  }

  IOException(const IOException& ex) {
    what_ = ex.what_;
  }

  const char *what() const noexcept override {
    return what_.c_str();
  }

private:
  std::string what_;
};
namespace duckdb {

}
}
#endif //INDEX_EXPLORER_STORAGE_INFO_H
