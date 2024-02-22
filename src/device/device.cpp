#include "graphloom/device/device.h"
#include "graphloom/op/op.h"

namespace graphloom
{

    /**
     * Device Impl
    */
   
    std::string Device::name() const
    {
        return name_;
    }

    std::string Device::type() const
    {
        return type_;
    }

    Device::Device()
    {
        omp_init_lock(&lock_);
    }

    Device::~Device()
    {
        omp_destroy_lock(&lock_);
    }

    Status Device::Compute(OpKernel* kernel, ComputeContext& context)
    {
        return kernel->Compute(context);
    }


    /**
     * DeviceFactory
    */

    DeviceFactory::DeviceFactory(const std::string& device_type) : 
        device_type_(device_type)
    {

    }

    std::string DeviceFactory::device_type() const
    {
        return device_type_;
    }
}