#ifndef GRAPHLOOM_TENSOR_TENSOR_H_
#define GRAPHLOOM_TENSOR_TENSOR_H_

#include <initializer_list>

#include "graphloom/common/status.h"
#include "graphloom/common/data_type.h"
#include "graphloom/device/device.h"

#define GRAPHLOOM_MAX_LAYOUT 16

/**
 * This module defines tensors an abstraction layer
 * ontop of continuous collections of memory.
*/

namespace graphloom
{
    /**
     * A fixed sized array that defines tensor layouts 
    */
    class LayoutArray
    {
    public:
        /**
         * @param list Layout size initalization list
        */
        LayoutArray(const std::initializer_list<size_t>& list);

        /**
         * @param x Size of rank 1
        */
        LayoutArray(size_t x);

        LayoutArray(const LayoutArray&)             = default;
        LayoutArray& operator=(const LayoutArray&)  = default;
        LayoutArray& operator=(const std::initializer_list<size_t>& list);

        // move deleted because there is nothing to move
        LayoutArray(LayoutArray&&)                  = delete;
        LayoutArray& operator=(LayoutArray&&)       = delete;

        /**
         * @returns Number of valid sizes
        */
        size_t rank() const;
        
        const size_t& operator[](size_t index) const;

        /**
         * Set the layout size
         * 
         * @param list New layout size list
        */
        void Set(const std::initializer_list<size_t>& list);

        size_t* begin() 
        {
            return array_;
        }

        size_t* end() {
            return array_ + rank();
        }


    private:
        size_t rank_;
        size_t array_[GRAPHLOOM_MAX_LAYOUT];
    };

    /**
     * Maps tensor to memory buffer
    */
    template<typename T>
    class TensorMap
    {
    public:

        /**
         * @param buf Underlying memory buffer
         * @param shape Shape of the tensor
         * @param strides Stride of each tensor's dimension
        */
        TensorMap(T* buf, const LayoutArray& shape, const LayoutArray& strides) : 
            buf_(buf),
            shape_(shape),
            strides_(strides)
        {
            if (shape_.rank() != strides_.rank())
            {
                throw GlException("Shape and stride ranks mismatched");
            }

            for (size_t dim : shape)
            {
                if (dim == 0)
                {
                    throw GlException("Shape cannot have 0 dim");
                }
            }
        }

        /**
         * 
         * @param buf Underlying memory buffer
         * @param shape Shape of the tensor
        */
        TensorMap(T* buf, const LayoutArray& shape) : 
            TensorMap(buf, shape, NaiveStrides(shape))
        {
            
        }

        template<typename... Args>
        T& operator()(size_t i, Args... args)
        {
            return *AccessHelper(strides_, i, args...); 
        }

    private:
        T* AccessHelper(size_t* strides, size_t i)
        {
            return buf_ + i*strides[0];
        }

        template<typename... Args>
        T* AccessHelper(size_t* strides, size_t i, Args... args)
        {
            return AccessHelper(strides + 1, args...) + i*strides[0];
        }

        /**
         * Generates naive stride from shape
         * 
         * @param shape Base shape
         * @returns generated strides
        */
        static LayoutArray NaiveStrides(const LayoutArray& shape)
        {
            LayoutArray strides;
            size_t stride = 1;
            for (int i = shape.rank() - 1; i >= 0; --i)
            {
                strides[i] = stride;
                stride *= shape[i];
            }
            return strides;
        }

        LayoutArray shape_;
        LayoutArray strides_;
        T* buf_;
    };
    
    /**
     * A memory buffer interpreted as a tensor
    */
    class TensorBuffer
    {
    public:
        /**
         * @param dtype Data type of each element in the tensor
         * @param shape Shape of the tensor
         * @param device The device the memory buffer lives on
        */
        TensorBuffer(DataType dtype, const LayoutArray& shape, Device* device);
        ~TensorBuffer();

        TensorBuffer(const TensorBuffer& other);
        TensorBuffer& operator=(const TensorBuffer& other);
        TensorBuffer(TensorBuffer&& other);
        TensorBuffer& operator=(TensorBuffer&& other);
        
        /**
         * @returns Shape of the tensor
        */
        const LayoutArray& shape() const;

        /**
         * @returns Number of elements
        */
        size_t size() const;

        /**
         * @returns Data type of the element
        */
        DataType dtype() const;

        /**
         * Reshapes the shape of the tensor. 
         * 
         * NOTE: The size of 
         * the new shape must equal current size
         * 
         * @param list New shape
         * @returns Reshape status
        */
        Status Reshape(const std::initializer_list<size_t>& list);

        /**
         * Creates a TensorMap to access the tensor data
         * 
         * @param T Type to interpret the buffer as
         * @returns TensorMap 
        */
        template<typename T>
        TensorMap<T> Map()
        {
            if (sizeof(T) != DataTypeSize(dtype()))
            {
                throw GlException("Bad cast, type size does not match");
            }
            return TensorMap(base<T>(), shape());
        }
        

    private:
        /**
         * @returns Underlying memory buffer
        */
        void* data();
        
        /**
         * @returns Pointer casted underlying memory buffer
        */
        template <typename T>
        T* base() {
            return reinterpret_cast<T*>(data());
        }
        
        Device* device_;
        LayoutArray shape_;
        size_t size_;
        void* data_;
        DataType dtype_;
    };
}


#endif