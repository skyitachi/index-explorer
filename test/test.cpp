#include <gtest/gtest.h>

#include "zap/utils.h"

TEST(ReadIntTest, ReadUInt) {
	{
		char buf[] = {0x0, 0x0, 0x0, 0x1};
		uint32_t ans = zap::readUInt32(buf, 0);
		ASSERT_EQ(ans, 1);
	}
	{
		char buf[] = {0x0, 0x0, 0x1, 0x2};
		uint32_t ans = zap::readUInt32(buf, 0);
		ASSERT_EQ(ans, 258);	
	}
	{
		char buf[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0b00101110, static_cast<char>(0b10110111)};
		uint64_t ans = zap::readUInt64(buf, 0);
		ASSERT_EQ(ans, 11959);
	}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}