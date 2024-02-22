#ifndef GRAPHLOOM_DEVICE_CPU_H_
#define GRAPHLOOM_DEVICE_CPU_H_

#include <omp.h>
#include <string>

#include "graphloom/device/device.h"
#include "graphloom/device/registration.h"

/**
 * This module defines the CPU device, CPU factory
 * and finally registers the device.
*/

namespace graphloom
{

    // Represents an instance of CPU device
    class Cpu : public Device
    {
    public:
        Status status() const override;
        Status malloc(DataType dtype, size_t size, void*& ptr) override;
        Status free(DataType dtype, void* ptr) override;
    };

    // Responsible for CPU device discovery
    class CpuFactory : public DeviceFactory
    {
    public:
        explicit CpuFactory(const std::string& device_type);
        Status DiscoverDevices(std::vector<Device*>& devices_found) override;
    };

    // Registers the factory to singleton of device factories.
    GL_REGISTER_DEVICE(CPU, CpuFactory);
}

#endif