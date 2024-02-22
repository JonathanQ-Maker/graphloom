#include "graphloom/common/status.h"
#include "graphloom/tensor/tensor.h"
#include "graphloom/device/device.h"


namespace graphloom
{
    /**
     * LayoutArray Impl
    */

    LayoutArray::LayoutArray(const std::initializer_list<size_t>& list)
    {
        Set(list);
    }

    LayoutArray::LayoutArray(size_t x)
    {
        Set({x});
    }

    LayoutArray& LayoutArray::operator=(const std::initializer_list<size_t>& list)
    {
        Set(list);
        return *this;
    }

    size_t LayoutArray::rank() const
    {
        return rank_;
    }

    const size_t& LayoutArray::operator[](size_t index) const
    {
        return array_[index];
    }

    void LayoutArray::Set(const std::initializer_list<size_t>& list)
    {
        if (list.size() > GRAPHLOOM_MAX_LAYOUT)
        {
            throw GlException("Only layout size ", GRAPHLOOM_MAX_LAYOUT, " is supported");
        }
        
        rank_ = list.size();

        size_t i = 0;
        for (size_t d : list)
        {
            array_[i++] = d;
        }
    }

    /**
     * TensorBuffer Impl
    */

    TensorBuffer::TensorBuffer(DataType dtype, const LayoutArray& shape, Device* device) :
        dtype_(dtype), shape_(shape), device_(device), data_(nullptr)
    {
        // compute size in bytes to allocate
        size_ = DataTypeSize(dtype);
        for (size_t i = 0; i < shape_.rank(); ++i)
        {
            size_ *= shape_[i];
        }
        
        GL_CHECK_OK(device->malloc(this->dtype(), this->size(), data_));
    }

    TensorBuffer::~TensorBuffer()
    {
        // no status checking because exceptions 
        // should be avoided in destructor
        device_->free(dtype_, data_);
        data_ = nullptr;
        size_ = 0;
    }
    
    const LayoutArray& TensorBuffer::shape() const
    {
        return shape_;
    }

    size_t TensorBuffer::size() const
    {
        return size_;
    }

    DataType TensorBuffer::dtype() const
    {
        return dtype_;
    }

    Status TensorBuffer::Reshape(const std::initializer_list<size_t>& list)
    {
        size_t size = DataTypeSize(dtype());
        for (size_t d : list)
        {
            size *= d;
        }
        if (size == this->size())
        {
            shape_ = list;
            return Status::kOK;
        }
        return Status(1, "Cannot reshape size=", this->size(), 
            " into size=", size);
    }

    void* TensorBuffer::data()
    {
        return data_;
    }

}