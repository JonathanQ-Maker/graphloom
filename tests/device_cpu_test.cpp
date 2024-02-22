#include <gtest/gtest.h>
#include <graphloom/graphloom.h>
#include <vector>
#include <string>

using namespace graphloom;

TEST(DeviceCPUSuite, ValidDevice) 
{
    const std::string cpu_name("CPU:0");

    EXPECT_TRUE(DeviceRegistry::instance().HasDevice(cpu_name))
        << "Expected device \"" << cpu_name << "\" but its not found with HasDevice().";

    Device* cpu = DeviceRegistry::instance().GetDevice(cpu_name);
    ASSERT_TRUE(cpu)
        << "Expected device \"" << cpu_name << "\" but its not found with GetDevice().";

    EXPECT_EQ(cpu->name(), cpu_name)
        << "CPU device self reported name is different than name registered in DeviceRegistry";

    Status status = cpu->status();
    EXPECT_TRUE(status.ok())
        << "CPU device status not ok, status msg=\"" << status.msg() << "\"";

    EXPECT_EQ(cpu->type(), "CPU")
        << "CPU device is not of type CPU";
}

TEST(DeviceCPUSuite, ValidAllocatorFloat) 
{
    const std::string cpu_name("CPU:0");
    const size_t size = 10000;

    Device* cpu = DeviceRegistry::instance().GetDevice(cpu_name);
    EXPECT_TRUE(cpu)
        << "Expected device \"" << cpu_name << "\" but its not found with GetDevice().";

    void* buffer = nullptr;
    Status status = cpu->malloc(DataType::Float, size, buffer);

    EXPECT_TRUE(status.ok())
        << "Expected ok malloc status, instead got: code=" 
        << status.code() << " \"" << status.msg() << "\"";
    
    ASSERT_TRUE(buffer)
        << "Expected successful memory allocation, instead buffer = nullptr";

    EXPECT_TRUE(reinterpret_cast<intptr_t>(buffer) % DataTypeSize(DataType::Float) == 0)
        << "Allocated buffer is not float aligned";

    // check memory size is at minimum correct by 
    // trying to trigger a segmentation fault
    float* float_buffer = static_cast<float*>(buffer);
    for (int i = 0; i < size; ++i)
    {
        float_buffer[i] = 1.0/(i+1);
    }
    float_buffer = nullptr;
    
    status = cpu->free(DataType::Float, buffer);
    buffer = nullptr;

    EXPECT_TRUE(status.ok())
        << "Expected ok free status, instead got: code=" 
        << status.code() << " \"" << status.msg() << "\"";
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}