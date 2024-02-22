#include <gtest/gtest.h>
#include <graphloom/graphloom.h>
#include <string>
#include <vector>
#include <utility>

using namespace graphloom;


GL_REGISTER_DEVICE(TEST_CPU, CpuFactory);
GL_REGISTER_DEVICE(TEST_CPU2, CpuFactory);
GL_REGISTER_DEVICE(TEST_CPU3, CpuFactory);

TEST(DeviceRegistrySuite, TEST_CPU)
{
    EXPECT_TRUE(DeviceRegistry::instance().HasDevice("TEST_CPU:0"))
        << "Expected TEST_CPU:0, but was not found";

    EXPECT_TRUE(DeviceRegistry::instance().HasDevice("TEST_CPU2:0"))
        << "Expected TEST_CPU2:0, but was not found";

    EXPECT_TRUE(DeviceRegistry::instance().HasDevice("TEST_CPU3:0"))
        << "Expected TEST_CPU3:0, but was not found";

    EXPECT_GE(DeviceRegistry::instance().num_factories(), 3)
        << "Expected at least 3 registered factories, but got " 
        << DeviceRegistry::instance().num_factories();
}

TEST(DeviceRegistrySuite, Singleton) 
{
    EXPECT_EQ(&DeviceRegistry::instance(), &DeviceRegistry::instance())
        << "Multiple calls to get DeviceRegistry::instance() returns different instances";
}

TEST(DeviceRegistrySuite, Devices)
{
    std::vector<std::string> devices = 
        DeviceRegistry::instance().GenDeviceList();

    EXPECT_GE(devices.size(), 3)
        << "Expected at least 3 device. Found " << devices.size();

    EXPECT_EQ(devices.size(), DeviceRegistry::instance().num_devices())
        << "Generated device list has different number of device reported by num_devices(). "
        << devices.size() << " != " << DeviceRegistry::instance().num_devices();

    for (const std::string& d : devices)
    {
        EXPECT_TRUE(DeviceRegistry::instance().HasDevice(d))
            << "Device \"" << d <<"\" listed in GenDeviceList() but does not exist.";

        EXPECT_NE(DeviceRegistry::instance().GetDevice(d), nullptr)
            << "Device \"" << d <<"\" listed in GenDeviceList() but get return nullptr.";
    }
}

TEST(DeviceRegistrySuite, DuplicateName)
{
    // Should fail and throw exception due to duplicate device type name
    EXPECT_THROW(RegisterDevice<CpuFactory>().Register("TEST_CPU"), GlException);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}