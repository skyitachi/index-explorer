//
// Created by Shiping Yao on 2023/1/4.
//

#include <sys/fcntl.h>
#include <unistd.h>
#include "MetaBlockReader.h"

namespace explorer {
namespace duckdb {

File::File(const std::string &file_path) {
  int fd = ::open(file_path.c_str(), O_RDONLY);
  if (fd < 0) {
    ::exit(-1);
  }
  fd_ = fd;
  offset_ = 0;
}

idx_t File::Read(data_ptr_t data, idx_t size) {
  auto st = ::pread(fd_, data, size, offset_);
  if (st < 0) {
    throw IOException(strerror(errno));
  }
  offset_ += st;
  return st;
}

idx_t File::Read(data_ptr_t data, idx_t size, idx_t offset) {
  auto st = ::pread(fd_, data, size, offset);
  if (st < 0) {
    throw IOException(strerror(errno));
  }
  return st;
}

void File::ReadData(data_ptr_t buffer, idx_t read_size) {
  Read(buffer, read_size);
}

const char MainHeader::MAGIC_BYTES[] = "DUCK";

void MainHeader::CheckMagicBytes(File &file) {
  data_t magic_bytes[MAGIC_BYTE_SIZE];
  file.Read(magic_bytes, MAGIC_BYTE_SIZE, MAGIC_BYTE_OFFSET);
  if (memcmp(magic_bytes, MAGIC_BYTES, MAGIC_BYTE_SIZE) != 0) {
    throw IOException("unexpect magic bytes");
  }
}

DatabaseHeader DatabaseHeader::ReadDatabaseHeader(Deserializer &deserializer) {
  DatabaseHeader header{};
  header.iteration = deserializer.Read<uint64_t>();
  header.meta_block = deserializer.Read<block_id_t>();
  header.free_list = deserializer.Read<block_id_t>();
  header.block_count = deserializer.Read<uint64_t>();
  return header;
}

}
}