#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "coding.h"
#include "segment.h"
#include "slice.h"
#include "utils.h"

namespace zap {
bool Segment::Open(const std::string &path) {
  if (is_opened_) {
    fprintf(stderr, "already opened");
    return false;
  }
  int fd;
  if ((fd = open(path.c_str(), O_RDWR)) < 0) {
    fprintf(stderr, "can't open %s \n", path.c_str());
    return false;
  }
  struct stat sbuf;
  if (fstat(fd, &sbuf) < 0) {
    fprintf(stderr, "fstat error\n");
    return false;
  }
  sz_ = sbuf.st_size;
  data_ = new char[sbuf.st_size];
  is_opened_ = true;
  void *src;
  if ((src = mmap(0, sz_, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
    fprintf(stderr, "mmap file error");
  }
  // TODO: 需要拷贝吗
  data_ = (char *)src;
  return true;
}

const Footer &Segment::parseFooter() {
  assert(is_opened_);
  int crcOffset = sz_ - 4;
  footer_.CC = readUInt32(data_, crcOffset);

  int verOffset = crcOffset - 4;
  footer_.Ver = readUInt32(data_, verOffset);

  int chunkOffset = verOffset - 4;
  footer_.CF = readUInt32(data_, chunkOffset);

  int docValueOffset = chunkOffset - 8;
  footer_.DOC = readUInt64(data_, docValueOffset);

  int fieldsIndexOffset = docValueOffset - 8;
  footer_.F = readUInt64(data_, fieldsIndexOffset);

  int storeIndexOffset = fieldsIndexOffset - 8;
  footer_.SF = readUInt64(data_, storeIndexOffset);

  int numDocsOffset = storeIndexOffset - 8;
  footer_.D = readUInt64(data_, numDocsOffset);

  return footer_;
}

bool Segment::loadFields() {
  assert(is_opened_);
  uint64_t fieldsIndexEnd = sz_ - KFooterSize;
  uint64_t fieldID = 0;
  uint64_t offset = footer_.F + 8 * fieldID;
  while (offset < fieldsIndexEnd) {
    uint64_t addr = readUInt64(data_, offset);
    Slice *slice_ = new Slice(data_ + addr, fieldsIndexEnd - addr);
    uint64_t dictLoc;
    bool is_success = GetVarint64(slice_, &dictLoc);
    if (!is_success) {
      return false;
    }
    dictLocs.push_back(dictLoc);
    uint64_t nameLen;
    is_success = GetVarint64(slice_, &nameLen);
    if (!is_success) {
      return false;
    }
    std::string name(slice_->data(), nameLen);
    fieldsInv.push_back(name);
    fieldsMap[name] = uint16_t(fieldID + 1);
    fieldID++;
    offset = footer_.F + 8 * fieldID;
    delete slice_;
  }
  return true;
}

} // namespace zap