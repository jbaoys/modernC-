#include <gtest/gtest.h>
#include <gmock/gmock.h>  // Brings in gMock.

#include "VmfAttribute.hh"

// Demonstrate some basic assertions.
TEST(VmfAttributeTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(VmfAttributeTest, IntAttribute_Constructor) {
    IntAttribute<int8_t, 4, -3, 10> myInt(-2);
    EXPECT_EQ(-2, myInt.getValue());
    EXPECT_EQ(4, myInt.getNumberOfBits());
}

TEST(VmfAttributeTest, IntAttribute_setget) {
    IntAttribute<int8_t, 4, -3, 10> myInt;
    EXPECT_FALSE(myInt.setValue(-4));
    EXPECT_TRUE(myInt.setValue(7));
    EXPECT_EQ(7, myInt.getValue());
    EXPECT_EQ(4, myInt.getNumberOfBits());
}

TEST(VmfAttributeTest, IntAttribute_setget2) {
    IntAttribute<int8_t, 7, -3, 10> myInt;
    std::array<int8_t, 1> data = {7};
    EXPECT_TRUE(myInt.setValue(7));
    EXPECT_EQ(data, myInt.getValues());
    IntAttribute<int8_t, 7, -3, 10, 3> myIntArray;
    std::array<int8_t, 3> data2 = {-1, 2, 7};
    EXPECT_TRUE(myIntArray.setValues(data2));
    //EXPECT_TRUE(myIntArray.setValues({-1,2,7}));
    std::array<int8_t, 3> readValues = myIntArray.getValues();
    for (int i=0; i<3; i++)
        EXPECT_EQ(data2[i], readValues[i]);
}

TEST(VmfAttributeTest, IntAttribute_encode) {
    std::array<int8_t, 3> data = {-1, 2, 7};
    IntAttribute<int8_t, 7, -3, 10> myInt(7);
    uint8_t tobuf[128];
    EXPECT_EQ(7, myInt.getValue());
    EXPECT_TRUE(myInt.encode(tobuf));
    std::cout << tobuf << '\n';
    IntAttribute<int8_t, 7, -3, 10, 3> myInt2({-1,2,7});
    EXPECT_EQ(data, myInt2.getValues());
    EXPECT_TRUE(myInt2.encode(tobuf));
    std::cout << tobuf << '\n';
}
