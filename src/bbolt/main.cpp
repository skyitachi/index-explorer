#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
	size_t sz = 4096;
	
	int fd = open("test.bbolt", O_RDWR | O_CREAT, 0644);
	if (fd < 0) {
		fprintf(stderr, "open file failed: %d\n", errno);
		return errno;
	}
	std::cout << "open file success" << std::endl;
	char* data;
	void *mapped;
	if ((mapped = mmap(0, sz * 2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "mmap file error\n");
		return errno;
	}
	data = reinterpret_cast<char *>(mapped);

	{
		int ret = madvise(data, sz, MADV_RANDOM);
		if (ret < 0) {
			fprintf(stderr, "madvise failed");
			return errno;
		}
	}
	
	std::cout << "mmap file success" << std::endl;

	const char* buf = "hello world";
	auto ret = pwrite(fd, buf, strlen(buf), sz);
	if (ret < 0) {
		fprintf(stderr, "write file failed: %d\n", errno);
		return errno;
	}	
	std::cout << "write file success" << std::endl;
	strcpy(data, "hello world");
	std::cout << std::string(data, strlen(buf)) << std::endl;
	std::cout << std::string(data + sz, strlen(buf)) << std::endl;
}