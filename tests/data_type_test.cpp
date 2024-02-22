#include <gtest/gtest.h>
#include <graphloom/graphloom.h>

using namespace graphloom;

TEST(DataTypeSuite, DataSizeTest)
{
    EXPECT_EQ(DataTypeSize(DataType::Double), sizeof(double))
        << "Expected size of Double mismatched";
    EXPECT_EQ(DataTypeSize(DataType::Float), sizeof(float))
        << "Expected size of Float mismatched";
    EXPECT_EQ(DataTypeSize(DataType::Int16), sizeof(int16_t))
        << "Expected size of Int16 mismatched";
    EXPECT_EQ(DataTypeSize(DataType::Int32), sizeof(int32_t))
        << "Expected size of Int32 mismatched";
    EXPECT_EQ(DataTypeSize(DataType::Int64), sizeof(int64_t))
        << "Expected size of Int64 mismatched";
    EXPECT_EQ(DataTypeSize(DataType::Int8), sizeof(int8_t))
        << "Expected size of Int8 mismatched";
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}