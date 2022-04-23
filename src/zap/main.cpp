#include "segment.h"
#include <iostream>

int main() {
  Segment seg_{};
  bool is_success = seg_.Open(
      "/Users/skyitachi/lab/index-explorer/cmake_build/src/000000000002.zap");

  if (is_success) {
    const Footer &footer = seg_.parseFooter();
    std::cout << footer.describe() << std::endl;
    return 0;
  }
  std::cout << "open failed" << std::endl;
}
