#ifndef GRAPHLOOM_DEVICE_REGISTRATION_H_
#define GRAPHLOOM_DEVICE_REGISTRATION_H_

#include <unordered_map>
#include <string>
#include <vector>
#include <type_traits>

#include "graphloom/device/device.h"
#include "graphloom/common/initializer.h"
#include "graphloom/common/status.h"
#include "graphloom/common/macros.h"

/**
 * This module defines the DeviceRegistry singleton and 
 * the helpers to register a new device.
*/

// Registers a new device to graphloom and defines a new DeviceType enum 
#define GL_REGISTER_DEVICE(device_type_name, device_factory)        \
    inline graphloom::Initializer gl_init_##device_type_name =      \
    graphloom::RegisterDevice<device_factory>()                     \
    .Register(GL_STRINGIFY(device_type_name))

namespace graphloom
{
    /**
     * A singleton that keeps track of all Device instances
     * and registered DeviceFactory
    */
    class DeviceRegistry
    {
    public:
        // Gets the singleton instance
        static DeviceRegistry& instance();

        /**
         * Returns true if a device instance with name exists.
         * Case sensitive
         * 
         * @param name device name to check
         * @returns Device existance
        */
        bool HasDevice(const std::string& name) const;

        /**
         * Gets device with name, throws 
         * exception if device does not exist.
         * Case sensitive
         * 
         * @param name Device instance name
         * @returns Device instance with given name
        */
        Device* GetDevice(const std::string& name) const;

        /**
         * Generates a list of device instances
         * 
         * @returns List of device instance names
        */
        std::vector<std::string> GenDeviceList() const;

        /**
         * @param type Name of the device type. Case sensitive
         * @returns True if device type exists
        */
        bool HasDeviceType(const std::string& type) const;

        /**
         * @returns Number of device instances
        */
        size_t num_devices() const;

        /**
         * @returns Number of factories registered
        */
       size_t num_factories() const;

    private:
        friend class GraphFactory;

        template<typename T>
        friend class RegisterDevice;

        /**
         * Generates a uniqe device name in the registry from a base name.
         * 
         * Given base "myDevice" possible names:
         * myDevice:0,
         * myDevice:1,
         * myDevice:2 ...
         * 
         * @param base_name Name to base on
         * @returns Unique device name
        */
        std::string GenUniqueName(const std::string& base_name);

       

        DeviceRegistry() = default;
        ~DeviceRegistry();
        DeviceRegistry(const DeviceRegistry&)               = delete;
        DeviceRegistry& operator=(const DeviceRegistry&)    = delete;

        std::unordered_map<std::string, Device*> devices_; // map of device name to device instance
        std::unordered_map<std::string, DeviceFactory*> factories_; // map of device type to DeviceFactory
    };
 
    /**
     * Builder to register DeviceFactory.
     * 
     * @param T Derived class of DeviceFactory
    */
    template<typename T>
    class RegisterDevice
    {
    public:
        RegisterDevice()
        {
            if (!std::is_base_of<DeviceFactory, T>::value)
            {
                throw GlException("Can only build with DeviceFactory derived type");
            }
        }

        /**
         * Finalize and register the new device.
         * 
         * @param name Name of the device type
         * @returns Initalizer
        */
        Initializer Register(const std::string& name) const
        {
            if (DeviceRegistry::instance().HasDeviceType(name))
            {
                throw GlException("Failed to register device, the device type name \"", name, "\" already exists.");
            }

            T* factory = new T(name);
            DeviceRegistry::instance().factories_.insert({name, factory});

            std::vector<Device*> devices_found;
            GL_CHECK_OK(factory->DiscoverDevices(devices_found));

            for (Device* d : devices_found)
            {
                d->type_ = factory->device_type();
                d->name_ = std::move(DeviceRegistry::instance().
                    GenUniqueName(factory->device_type()));
                DeviceRegistry::instance().devices_.insert({d->name_, d});
            }

            return Initializer();
        }
    };
}

#endif