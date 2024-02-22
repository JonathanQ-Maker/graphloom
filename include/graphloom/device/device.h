#ifndef GRAPHLOOM_DEVICE_DEVICE_H_
#define GRAPHLOOM_DEVICE_DEVICE_H_

#include <string>
#include <vector>
#include <omp.h>

#include "graphloom/common/status.h"
#include "graphloom/common/data_type.h"
#include "graphloom/graph/graph_context.h"

/**
 * This module defines the base classes used to 
 * represent computation devices. 
 * 
 * These classes primarily serve to support device 
 * specific kernel implementations. 
 * 
 * The DeviceFactory class is intended to be inherited
 * to represent 
 *
*/

namespace graphloom
{
    class OpKernel;

    /**
     * The Device class is intended to be inherited to represent 
     * an instance of a device.
    */
    class Device
    {
    public:

        /**
         * Reports the current status of the device
         * 
         * @returns Device status
        */
        virtual Status status() const = 0;

        /**
         * Returns the name given to the device
         * 
         * @returns Device name
        */
        std::string name() const;

        /**
         * Returns the device type.
         * 
         * Ex. CPU device or CUDA device
         * 
         * @returns Device type
        */
        std::string type() const;

        /**
         * Allocates size*DataTypeSize(dtype) byte buffer on this 
         * device and returns the buffer pointer into ptr.
         * 
         * NOTE: Must be thread safe
         * 
         * @param dtype Element type
         * @param size Number of data elements
         * @param ptr Returned buffer pointer
         * 
         * @returns Allocation status
        */
        virtual Status malloc(DataType dtype, size_t size, void*& ptr) = 0;

        /**
         * Frees the buffer allocated on this device.
         * 
         * NOTE: dtype is the dtype of the buffer to be freed
         * NOTE: Must be thread safe
         * 
         * @param dtype Element type
         * @param ptr Returned buffer pointer
         * @returns Memory free status
        */
        virtual Status free(DataType dtype, void* ptr) = 0;

        // delete copy and move to ensure single 
        // instance per physical device
        Device(const Device&)             = delete;
        Device& operator=(const Device&)  = delete;
        Device(Device&&)                  = delete;
        Device& operator=(Device&&)       = delete;

    protected:        
        /**
         * @param type Device type
        */
        Device();
        virtual ~Device();
        
        /**
         * Computes the provided kernel on this device
         * 
         * @param context Execution context
         * @returns Compute status
        */
        virtual Status Compute(OpKernel* kernel, ComputeContext& context);

        omp_lock_t lock_; // mutex/lock for thread safety

    private:
        friend class DeviceFactory;
        template<typename T>
        friend class RegisterDevice;
        friend class DeviceRegistry;

        std::string name_; // device name
        std::string type_; // device type
    };

    /**
     * The DeviceFactory is responsible for discovery 
     * and initalization of a specific device type. 
     * 
     * For every device type there is exactly one 
     * corresponding DeviceFactory instance
     * 
     * DeviceFactory must be registered to DeviceRegistry 
     * to function.
    */
    class DeviceFactory
    {
    public:

        /**
         * @param device_type Device type
        */
        DeviceFactory(const std::string& device_type);

        /**
         * @returns Device type of this factory
        */
        std::string device_type() const;

        /**
         * Called during initalization to discover and 
         * initalize all matching device types
         * 
         * @param devices_found list to be filled with 
         * found devices
         * 
         * @returns device discovery status
        */
        virtual Status DiscoverDevices(std::vector<Device*>& devices_found) = 0;

    private:
        template<typename T>
        friend class RegisterDevice;

        const std::string device_type_; // device type
    };
}

#endif
