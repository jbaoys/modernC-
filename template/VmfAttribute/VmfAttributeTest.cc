#include <gtest/gtest.h>
#include <gmock/gmock.h>  // Brings in gMock.

#include "VmfAttribute.hh"
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
    int8_t setValues[3] = {-64, 0, 63};
    EXPECT_TRUE(myInt7Array.setValues(setValues));
    int8_t readValues[3];
    myInt7Array.getValues(readValues);
    for (int i=0; i<3; i++)
        EXPECT_EQ(setValues[i], readValues[i]);
}

TEST(VmfAttributeTest, IntAttribute_encode) {
    IntAttribute<int8_t, 7, -64, 63> myInt7(35);
    uint8_t tobuf[128];
    EXPECT_EQ(35, myInt7.getValue());
    EXPECT_TRUE(myInt7.encode(tobuf));
    std::cout << tobuf << '\n';
}

TEST(VmfAttributeTest, IntAttribute_encodeArray) {
    std::array<int8_t,3> varify = {-64, 0, 63};
    IntAttribute<int8_t, 7, -64, 63, 3> myInt7Array({-64,0,63});
    uint8_t tobuf[128];
    EXPECT_EQ(varify, myInt7Array.getValues());
    EXPECT_TRUE(myInt7Array.encode(tobuf));
    std::cout << tobuf << '\n';
}
#if 0
#endif
