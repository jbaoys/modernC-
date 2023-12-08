#include <gtest/gtest.h>
#include <gmock/gmock.h>  // Brings in gMock.

#include "VmfAttributeBuffer.h"

TEST(VmfAttributeBufferTest, Constructor) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);
    EXPECT_EQ(256, myBuffer.getSize());
    EXPECT_EQ(0, myBuffer.getContentSize());
}

TEST(VmfAttributeBufferTest, getContentSize) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);
    EXPECT_EQ(256, myBuffer.getSize());
    EXPECT_EQ(0, myBuffer.getContentSize());
    uint8_t bits[] = { 0, 1, 1, 0, 0, 0,1,0,1};
    for (auto b : bits) {
        EXPECT_TRUE(myBuffer.appendBit(b));
    }
    EXPECT_EQ(2, myBuffer.getContentSize());
    EXPECT_EQ(9, myBuffer.getContentBitSize());
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

TEST(VmfAttributeBufferTest, appendBits) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 2);
    EXPECT_FALSE(myBuffer.appendBits(0x8877665544332211, 17));
    myBuffer.purgeBuffer();
    EXPECT_TRUE(myBuffer.appendBits(0x8877665544332211, 16));
    myBuffer.purgeBuffer();
    EXPECT_TRUE(myBuffer.appendBits(0x8877665544332211, 1));
    EXPECT_TRUE(myBuffer.appendBits(0x8877665544332211, 0));
    VmfAttributeBuffer myBuffer2(buf, 3);
    EXPECT_FALSE(myBuffer2.setContentBitSize(25));
    EXPECT_TRUE(myBuffer2.setContentBitSize(24));
    EXPECT_TRUE(myBuffer2.setContentBitSize(23));
    myBuffer2.resetIterator();
    EXPECT_TRUE(myBuffer2.appendBits(0x8877665544332211, 0));
    myBuffer2.purgeBuffer();
    EXPECT_TRUE(myBuffer2.appendBits(0x8877665544332211, 23));
    myBuffer2.purgeBuffer();
    EXPECT_FALSE(myBuffer2.appendBits(0x8877665544332211, 25));
    myBuffer2.purgeBuffer();
    EXPECT_TRUE(myBuffer2.appendBits(0x8877665544332211, 24));
    EXPECT_EQ(3, myBuffer2.getContentSize());
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer2.getContentSize()));

    VmfAttributeBuffer myBuffer3(buf, 8);
    EXPECT_TRUE(myBuffer3.appendBits(0xabcdef, 7));
    EXPECT_TRUE(myBuffer3.appendBits(0x1, 1));
    EXPECT_FALSE(myBuffer3.appendBits(0x8877665544332211, 64));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    EXPECT_FALSE(myBuffer3.appendBits(0x8877665544332211, 57));
    EXPECT_TRUE(myBuffer3.appendBits(0x8877665544332211, 50));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    EXPECT_TRUE(myBuffer3.appendBits(-1, 6));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
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
