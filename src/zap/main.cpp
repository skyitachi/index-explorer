#include <iostream>
#include <folly/FBString.h>
#include <folly/FileUtil.h>
#include <vector>

int main() {
  std::cout << "hello zap" << std::endl;
  folly::fbstring str1 = "hello world";
  folly::ByteRange range;
  std::vector<uint8_t> index_content;
  
  std::cout << str1 << std::endl;
}
