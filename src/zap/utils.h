#include <iostream>

int32_t readInt32(const char* data_, int off) {
	return (data_[off] << 24)+ (data_[off + 1] << 16) + 
		(data_[off + 2] << 8) + data_[off + 3];
}

uint32_t readUInt32(const char* data_, int off) {
	return (data_[off] << 24)+ (data_[off + 1] << 16) + 
		(data_[off + 2] << 8) + data_[off + 3];
}

uint64_t readUInt64(const char* data_, int off) {
	uint64_t h = readUInt32(data_, off);
	uint64_t l = readUInt32(data_, off + 4);
	return (h << 32) + l;
}