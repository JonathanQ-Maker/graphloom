#ifndef GRAPHLOOM_COMMON_DATA_TYPE_H_
#define GRAPHLOOM_COMMON_DATA_TYPE_H_

#include <unordered_map>
#include <cstdint>

/**
 * This module defines the DataType enum 
 * and its helpers.
 * This allows graphloom to easily pass 
 * data type itself as a parameter/.
*/

namespace graphloom 
{
    enum DataType
    {
        Float,
        Double,

        Int8,
        Int16,
        Int32,
        Int64,
    };

    /**
     * DataType size lookup table. The literal value
     * of the enum must match the index of its size.
     */
    const size_t kDataTypeSize[] = {
        sizeof(float),
        sizeof(double),
        sizeof(int8_t),
        sizeof(int16_t),
        sizeof(int32_t),
        sizeof(int64_t)
    };

    /**
     * Returns the size of dtype with lookup table
     * 
     * @param dtype the type to lookup
     * @returns size of the type
    */
    inline size_t DataTypeSize(DataType dtype)
    {
        return kDataTypeSize[dtype];
    }
}

#endif