#include <cstdlib>

#include "graphloom/device/cpu.h"

namespace graphloom
{
    Status Cpu::status() const
    {
        return Status::kOK;
    }

    Status Cpu::malloc(DataType dtype, size_t size, void*& ptr)
    {
        omp_set_lock(&lock_);
        size_t dsize = DataTypeSize(dtype);
        ptr = static_cast<void*>(::operator new(size*dsize, std::align_val_t(dsize), std::nothrow));
        if (ptr == nullptr)
        {
            omp_unset_lock(&lock_); // to ensure the null check is atomic
            return Status(1, name(), " memory allocation failure");
        }
        
        omp_unset_lock(&lock_);
        return Status::kOK;
    }

    Status Cpu::free(DataType dtype, void* ptr)
    {
        omp_set_lock(&lock_);
        ::operator delete(ptr, std::align_val_t(DataTypeSize(dtype)));
        omp_unset_lock(&lock_);
        return Status::kOK;
    }

    CpuFactory::CpuFactory(const std::string& device_type):
        DeviceFactory(device_type) 
    {
        
    }

    Status CpuFactory::DiscoverDevices(std::vector<Device*>& devices_found)
    {
        devices_found.push_back(new Cpu());
        return Status::kOK;
    }
}