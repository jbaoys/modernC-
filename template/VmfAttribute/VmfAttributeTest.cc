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
    IntAttribute<int8_t, 7, -64, 63> myInt7(-64);
    EXPECT_EQ(-64, myInt7.getValue());
    IntAttribute<int8_t, 8, -128, 127> myInt8(-128);
    EXPECT_EQ(-128, myInt8.getValue());
    IntAttribute<int16_t, 16, -32768, 32767> myInt15(-32768);
    EXPECT_EQ(-32768, myInt15.getValue());
    IntAttribute<int32_t, 32, -2147483648, 2147483647> myInt32(-2147483648);
    EXPECT_EQ(-2147483648, myInt32.getValue());
    IntAttribute<int64_t, 40, -549755813888, 549755813887> myInt64(-549755813888);
    EXPECT_EQ(-549755813888, myInt64.getValue());
    //EXPECT_EQ(4, myInt.getNumberOfBits());
    /*IntAttribute<int8_t, 4, -3, 10, 3> myInt2({-2,3,7});
    EXPECT_EQ(-2, myInt2.getValue());
    EXPECT_EQ(12, myInt2.getNumberOfBits());
    int8_t data2[3] = {-2,3,7};
    int8_t readValues[3];
    myInt2.getValues(readValues);
    for (int i=0; i<3; i++)
        EXPECT_EQ(data2[i], readValues[i]);
        */
    //IntAttribute<int32_t, 20, -524289, 524288> myInt3(-524289);
    //EXPECT_EQ(-524289, myInt3.getValue());
}
#if 0
TEST(VmfAttributeTest, IntAttribute_setget) {
    IntAttribute<int8_t, 4, -3, 10> myInt;
    EXPECT_FALSE(myInt.setValue(-4));
    EXPECT_TRUE(myInt.setValue(7));
    EXPECT_EQ(7, myInt.getValue());
    EXPECT_EQ(4, myInt.getNumberOfBits());
}

TEST(VmfAttributeTest, IntAttribute_setget2) {
    IntAttribute<int8_t, 7, -3, 10> myInt;
    EXPECT_TRUE(myInt.setValue(7));
    EXPECT_EQ(7, myInt.getValue());
    IntAttribute<int8_t, 7, -3, 10, 3> myIntArray;
    //std::array<int8_t, 3> data2 = {-1, 2, 7};
    int8_t data2[3] = {-1, 2, 7};
    //EXPECT_TRUE(myIntArray.setValues(data2));
    EXPECT_TRUE(myIntArray.setValues(data2));
    int8_t readValues[3];
    myIntArray.getValues(readValues);
    for (int i=0; i<3; i++)
        EXPECT_EQ(data2[i], readValues[i]);
}

TEST(VmfAttributeTest, IntAttribute_encode) {
    int8_t data[3] = {-1, 2, 7};
    IntAttribute<int8_t, 7, -3, 10> myInt(7);
    uint8_t tobuf[128];
    EXPECT_EQ(7, myInt.getValue());
    EXPECT_TRUE(myInt.encode(tobuf));
    std::cout << tobuf << '\n';
    IntAttribute<int8_t, 7, -3, 10, 3> myInt2({-1,2,7});
    int8_t readValues[3];
    myInt2.getValues(readValues);
    for (int i=0; i<3; i++)
        EXPECT_EQ(data[i], readValues[i]);
    EXPECT_TRUE(myInt2.encode(tobuf));
    std::cout << tobuf << '\n';
}
#endif
