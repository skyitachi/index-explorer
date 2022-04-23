#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "segment.h"
#include "utils.h"

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