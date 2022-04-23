#include <bitset>
#include <iostream>

#include "coding.h"
#include "segment.h"

using namespace zap;

int main() {
  Segment seg_{};
  bool is_success = seg_.Open(
      "/Users/skyitachi/lab/index-explorer/cmake_build/src/000000000002.zap");

  if (is_success) {
    const Footer &footer = seg_.parseFooter();
    std::cout << footer.describe() << std::endl;
  }

  if (!seg_.loadFields()) {
    std::cout << "load fields failed" << std::endl;
  }
}
