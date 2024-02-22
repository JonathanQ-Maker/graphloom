#include "graphloom/device/registration.h"

namespace graphloom
{
    /**
     * DeviceRegistry Impl
    */

    DeviceRegistry& DeviceRegistry::instance()
    {
        static DeviceRegistry instance_;
        return instance_;
    }

    bool DeviceRegistry::HasDevice(const std::string& name) const
    {
        return devices_.find(name) != devices_.end();
    }

    Device* DeviceRegistry::GetDevice(const std::string& name) const
    {
        return devices_.at(name);
    }

    std::vector<std::string> DeviceRegistry::GenDeviceList() const
    {
        std::vector<std::string> result;
        result.reserve(devices_.size());
        for (auto pair : devices_)
        {
            result.push_back(pair.first);
        }
        return result;
    }

    bool DeviceRegistry::HasDeviceType(const std::string& type) const
    {
        return factories_.find(type) != factories_.end();
    }

    size_t DeviceRegistry::num_devices() const
    {
        return devices_.size();
    }

    size_t DeviceRegistry::num_factories() const
    {
        return factories_.size();
    }

    std::string DeviceRegistry::GenUniqueName(const std::string& base_name)
    {
        std::string name(base_name);

        // handle edge case index 0
        name += ":0";
        if (!HasDevice(name))
        {
            return name;
        }
        name = base_name;

        for (int i = 0; HasDevice(name); ++i)
        {
            name = base_name;
            name += ":";
            name += i;
        }
        return name;
    }

    DeviceRegistry::~DeviceRegistry()
    {
        for (auto pair : devices_)
        {
            delete pair.second;
        }
        devices_.clear();

        for (auto pair : factories_)
        {
            delete pair.second;
        }
        factories_.clear();
    }
}