#include <gtest/gtest.h>
#include <gmock/gmock.h>  // Brings in gMock.

#include "VmfAttribute.h"
#if 0
class VmfAttributeTest : public ::testing::TestWithParam<std::tuple<uint8_t, int8_t, int8_t>> {
   protected:
};

TEST_P(VmfAttributeTest, IntAttribute_Constructor) {
    uint8_t numbits = std::get<0>(GetParam());
    int8_t min = std::get<1>(GetParam());
    int8_t max = std::get<2>(GetParam());

    IntAttribute<int8_t, numbits, min, max> myInt(-2);
    EXPECT_EQ(-2, myInt.getValue());
    EXPECT_EQ(numbits, myInt.getNumberOfBits());

}

INSTANTIATE_TEST_CASE_P(
        VmfAttributeTestTitle,
        VmfAttributeTest,
        ::testing::Values(
            std::make_tuple(7, -64, 63),
            std::make_tuple(4, -8, 7)));
#endif

TEST(VmfAttributeTest, IntAttribute_Constructor) {
    IntAttribute<int8_t, 1, -1, 0> myInt1(-1);
    EXPECT_EQ(1, myInt1.getNumberOfBits());
    EXPECT_EQ(-1, myInt1.getValue());

    IntAttribute<int8_t, 7, -64, 63> myInt7(-64);
    EXPECT_EQ(7, myInt7.getNumberOfBits());
    EXPECT_EQ(-64, myInt7.getValue());

    IntAttribute<int8_t, 8, -128, 127> myInt8(-128);
    EXPECT_EQ(8, myInt8.getNumberOfBits());
    EXPECT_EQ(-128, myInt8.getValue());

    IntAttribute<int16_t, 16, -32768, 32767> myInt16(-32768);
    EXPECT_EQ(16, myInt16.getNumberOfBits());
    EXPECT_EQ(-32768, myInt16.getValue());

    IntAttribute<int32_t, 32, -2147483648, 2147483647> myInt32(-2147483648);
    EXPECT_EQ(32, myInt32.getNumberOfBits());
    EXPECT_EQ(-2147483648, myInt32.getValue());

    IntAttribute<int64_t, 40, -549755813888, 549755813887> myInt40(-549755813888);
    EXPECT_EQ(40, myInt40.getNumberOfBits());
    EXPECT_EQ(-549755813888, myInt40.getValue());
}

TEST(VmfAttributeTest, IntAttribute_ArrayConstructor) {
    IntAttribute<int8_t, 3, -3, 3, 3> myInt3Array({-3,0,3});
    EXPECT_EQ(9, myInt3Array.getNumberOfBits());
    std::array<int8_t, 3> data = {-3,0,3};
    EXPECT_EQ(data, myInt3Array.getValues());
}


TEST(VmfAttributeTest, IntAttribute_setget) {
    IntAttribute<int8_t, 4, -3, 7> myInt;
    EXPECT_EQ(4, myInt.getNumberOfBits());
    EXPECT_FALSE(myInt.setValue(-4));
    EXPECT_FALSE(myInt.setValue(8));
    EXPECT_TRUE(myInt.setValue(7));
    EXPECT_EQ(7, myInt.getValue());
}

TEST(VmfAttributeTest, IntAttribute_setgetArray) {
    IntAttribute<int8_t, 7, -64, 63, 3> myInt7Array;
    EXPECT_EQ(21, myInt7Array.getNumberOfBits());
    //int8_t setValues[4] = {-64, 0, 63, 37};
    //error: no matching function for call to ‘IntAttribute<signed char, 7, -64, 63, 3>::setValues(int8_t [4])’
    int8_t setValues[3] = {-64, 0, 63};
    EXPECT_TRUE(myInt7Array.setValues(setValues));
    int8_t readValues[3];
    myInt7Array.getValues(readValues);
    for (int i=0; i<3; i++)
        EXPECT_EQ(setValues[i], readValues[i]);
}

TEST(VmfAttributeTest, IntAttribute_encode) {
    IntAttribute<int8_t, 7, -64, 63> myInt7(35);
    const size_t sz = 32;
    uint8_t buf[sz];
    VmfAttributeBuffer tobuf(buf, sz);
    tobuf.purgeBuffer();
    EXPECT_EQ(35, myInt7.getValue());
    EXPECT_TRUE(myInt7.encode(tobuf));
    EXPECT_EQ(0x23, buf[0]);
    VmfDebug("Buffer content:\n", hexDataStr(buf,sz));
}

TEST(VmfAttributeTest, IntAttribute_encodeArray) {
    std::array<int8_t,3> varify = {-64, 0, 63};
    IntAttribute<int8_t, 7, -64, 63, 3> myInt7Array({-64,0,63});
    const size_t sz = 32;
    uint8_t buf[sz];
    VmfAttributeBuffer tobuf(buf, sz);
    tobuf.purgeBuffer();
    EXPECT_EQ(varify, myInt7Array.getValues());
    EXPECT_TRUE(myInt7Array.encode(tobuf));
    EXPECT_EQ(0x40, buf[0]);
    EXPECT_EQ(0xC0, buf[1]);
    EXPECT_EQ(0x0F, buf[2]);
    VmfDebug("Buffer content:\n", hexDataStr(buf,sz));
}

TEST(VmfAttributeTest, IntAttribute_decode) {
    IntAttribute<int8_t, 7, -64, 63> myInt7;
    const size_t sz = 32;
    uint8_t buf[sz] = { 0x40, 0xC0, 0x0F };
    VmfAttributeBuffer buffer(buf, sz);
    buffer.setContentSize(3);
    buffer.resetIterator();

    EXPECT_EQ(0, myInt7.getValue());
    EXPECT_TRUE(myInt7.decode(buffer));
    EXPECT_EQ(-64, myInt7.getValue());
    EXPECT_TRUE(myInt7.decode(buffer));
    EXPECT_EQ(0, myInt7.getValue());
    EXPECT_TRUE(myInt7.decode(buffer));
    EXPECT_EQ(63, myInt7.getValue());
}

TEST(VmfAttributeTest, IntAttribute_decodeArray) {
    std::array<int8_t,3> varify0 = {0, 0, 0};
    std::array<int8_t,3> varify = {-64, 0, 63};
    IntAttribute<int8_t, 7, -64, 63, 3> myInt7Array;
    const size_t sz = 32;
    uint8_t buf[sz] = { 0x40, 0xC0, 0x0F };
    VmfAttributeBuffer buffer(buf, sz);
    buffer.setContentSize(3);
    buffer.resetIterator();

    EXPECT_EQ(varify0, myInt7Array.getValues());
    EXPECT_TRUE(myInt7Array.decode(buffer));
    EXPECT_EQ(varify, myInt7Array.getValues());
}

TEST(VmfAttributeTest, IntAttribute_unsignT) {
    enum e0 {};
    unsignT<e0> a0;
    a0 = unsignT<e0>(-1);
    VmfDebug("unsingT<e0> of a0 = ", a0, '\n');
    VmfDebug("minVal<uint8_t,8>=", int64_t(minVal<uint8_t,8>()), '\n');
    VmfDebug("maxVal<uint8_t,8>=", int64_t(maxVal<uint8_t,8>()), '\n');
    VmfDebug("minVal<uint8_t,7>=", int64_t(minVal<uint8_t,7>()), '\n');
    VmfDebug("maxVal<uint8_t,4>=", int64_t(maxVal<uint8_t,4>()), '\n');
    VmfDebug("minVal<int8_t,8>=", int64_t(minVal<int8_t,8>()), '\n');
    VmfDebug("maxVal<int8_t,8>=", int64_t(maxVal<int8_t,8>()), '\n');
    VmfDebug("minVal<int8_t,7>=", int64_t(minVal<int8_t,7>()), '\n');
    VmfDebug("maxVal<int8_t,4>=", int64_t(maxVal<int8_t,4>()), '\n');
    VmfDebug("minVal<int,3>=", minVal<int,3>(), '\n');
    VmfDebug("maxVal<int,3>=", int64_t(maxVal<int,3>()), '\n');
    VmfDebug("minVal<e0,3>=", minVal<e0,3>(), '\n');
    VmfDebug("maxVal<e0,3>=", int64_t(maxVal<e0,3>()), '\n');
}

