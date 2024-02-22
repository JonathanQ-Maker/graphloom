#include "graphloom/op/op.h"

namespace graphloom
{
    /**
     * Op Impl
    */

    size_t Op::num_inputs() const
    {
        return num_inputs_;
    }

    size_t Op::num_outputs() const
    {
        return num_outputs_;
    }

    std::string Op::name() const
    {
        return name_;
    }

    const std::set<std::string>& Op::attributes() const
    {
        return attributes_;
    }


    /**
     * OpKernelDef Impl
    */

    std::string OpKernelDef::device() const
    {
        return device_;
    }

    DataType OpKernelDef::input_dtype(size_t index) const
    {
        return in_dtypes_.at(index);
    }

    DataType OpKernelDef::output_dtype(size_t index) const
    {
        return out_dtypes_.at(index);
    }

    size_t OpKernelDef::num_inputs() const
    {
        return in_dtypes_.size();
    }

    size_t OpKernelDef::num_outputs() const
    {
        return out_dtypes_.size();
    }

    OpKernelDef::OpKernelDef(const std::string& device, const std::function<OpKernel*(const OpKernelContext&)>& create_fn, 
            const std::vector<DataType>& in_dtypes, 
            const std::vector<DataType>& out_dtypes) :
            device_(device),
            create_fn_(create_fn),
            in_dtypes_(in_dtypes),
            out_dtypes_(out_dtypes)
    {

    }
    


}