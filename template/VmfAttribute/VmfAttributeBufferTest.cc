#include <gtest/gtest.h>
#include <gmock/gmock.h>  // Brings in gMock.

#include "VmfAttributeBuffer.h"

TEST(VmfAttributeBufferTest, Constructor) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);
    EXPECT_EQ(256, myBuffer.getSize());
    EXPECT_EQ(0, myBuffer.getContentSize());
}

TEST(VmfAttributeBufferTest, appendBit) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x01, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x03, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x03, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x03, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x13, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x13, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x13, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x93, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x93, buf[0]);
    EXPECT_EQ(0x01, buf[1]);
    EXPECT_EQ(2, myBuffer.getContentSize());
}

TEST(VmfAttributeBufferTest, getBit) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(1));
    myBuffer.resetIterator();
    uint8_t bit = 0;
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_FALSE(myBuffer.getBit(bit));
}
