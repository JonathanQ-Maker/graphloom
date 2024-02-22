#include <utility>

#include "graphloom/op/registration.h"

namespace graphloom
{
    /**
     * OpBuilder Impl
    */

    OpBuilder& OpBuilder::Input()
    {
        ++num_inputs_;
        return *this;
    }

    OpBuilder& OpBuilder::Output(const std::function<Status(const ComputeContext&, LayoutArray&)>& shape_fn)
    {
        ++num_outputs_;
        out_shape_fns_.push_back(shape_fn);
        return *this;
    }

    OpBuilder& OpBuilder::Attribute(const std::string& name)
    {
        auto pair = attributes_.insert(name);
        if (!pair.second)
        {
            throw GlException("Failed to add attribute \"", name, "\" because it already exists.");
        }
        return *this;
    }

    Initializer OpBuilder::Build()
    {
        Op op;
        op.out_shape_fns_   = std::move(out_shape_fns_);
        op.num_inputs_      = num_inputs_;
        op.num_outputs_     = num_outputs_;
        op.name_            = op_name_;
        op.attributes_      = std::move(attributes_);

        // reset
        num_inputs_     = 0;
        num_outputs_    = 0;

        Status status = OpRegistry::instance().RegisterOp(std::move(op));
        GL_CHECK_OK(status);
        return Initializer();
    }
    
    OpBuilder::OpBuilder(const std::string& op_name) :
        op_name_(op_name)
    {

    }


    /**
     * OpReistry Impl
    */

    OpRegistry& OpRegistry::instance()
    {
        static OpRegistry instance_;
        return instance_;
    }

    const Op& OpRegistry::GetOp(const std::string& name) const
    {
        return ops_.at(name);
    }
    
    bool OpRegistry::HasOp(const std::string& name) const
    {
        return ops_.find(name) != ops_.end();
    }

    size_t OpRegistry::size() const
    {
        return ops_.size();
    }

    Status OpRegistry::RegisterOp(const Op&& op)
    {
        if (HasOp(op.name()))
        {
            return Status(1, "Failed to register op \"", op.name(), 
                "\", name already exists");
        }

        ops_.insert(std::make_pair(op.name(), std::move(op)));
        return Status::kOK;
    }

    Status OpRegistry::RegisterOpKernel(const std::string& op_name, 
        OpKernelDef&& kernel)
    {
        if (!HasOp(op_name)) 
        {
            return Status(1, "Failed to register kernel. \"", op_name, "\" does not exist");
        }
        
        Op& op = ops_.at(op_name);
        
        if (op.num_inputs() != kernel.num_inputs())
        {
            return Status(2, "Failed to register kernel. Number of inputs mismatched ",
                op.num_inputs(), " != ", kernel.num_inputs());
        }

        if (op.num_outputs() != kernel.num_outputs())
        {
            return Status(3, "Failed to register kernel. Number of outputs mismatched ",
                op.num_outputs(), " != ", kernel.num_outputs());
        }

        op.kernels_.push_back(std::move(kernel));

        return Status::kOK;
    }
}