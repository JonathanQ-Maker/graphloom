#ifndef GRAPHLOOM_OP_REGISTRATION_H_
#define GRAPHLOOM_OP_REGISTRATION_H_

#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>

#include "graphloom/op/op.h"
#include "graphloom/common/macros.h"
#include "graphloom/common/status.h"
#include "graphloom/common/initializer.h"
#include "graphloom/op/registration.h"

/** 
 * This file defines modules nessesary to 
 * build and register ops and op kernels.
*/

// GL_REGISTER_OP implementation
#define GL_REGISTER_OP_IMPL(op_name, counter) \
    inline graphloom::Initializer GL_CONCATENATE(register_op_, counter) = \
    graphloom::OpBuilder(op_name)

// Register Op builder
#define GL_REGISTER_OP(op_name) \
    GL_REGISTER_OP_IMPL(op_name, __COUNTER__)

// GL_REGISTER_KERNEL implementation
#define GL_REGISTER_KERNEL_IMPL(op_name, kernel, device, counter) \
    inline graphloom::Initializer GL_CONCATENATE(register_kernel_, counter) = \
    graphloom::OpKernelDefBuilder<kernel>(op_name, device)

// Register Kernel builder
#define GL_REGISTER_KERNEL(op_name, kernel, device) \
    GL_REGISTER_KERNEL_IMPL(op_name, kernel, device, __COUNTER__)

namespace graphloom
{
    /**
     *  Builder that builds an op step by step
    */
    class OpBuilder
    {
    public:

        /**
         * Adds an input tensor to this op
         * 
         * @returns This builder
        */
        OpBuilder& Input();

        /**
         * Adds an output tensor to this op
         * 
         * @returns This builder
        */
        OpBuilder& Output(const std::function<Status(const ComputeContext&, LayoutArray&)>& shape_fn);

        /**
         * Adds an attribute with name
         * 
         * @param name The new attribute's name
         * @returns This builder
        */
        OpBuilder& Attribute(const std::string& name);
        
        /**
         * Finalize and build the result into OpRegistry singleton
         * 
         * @param Shape function that computes the shape of all output tensors
         * @returns Initializer object to allow out-of-main initalization
        */
        Initializer Build();
        
        /**
         * @param op_name The new op's name
        */
        OpBuilder(const std::string& op_name);

    private:
        size_t num_inputs_ = 0;
        size_t num_outputs_ = 0;
        std::string op_name_;
        std::set<std::string> attributes_;

        // list of functions that computes the shape of output 
        // tensor at their respective index
        std::vector<std::function<Status(const ComputeContext&, LayoutArray&)>> out_shape_fns_;
    };

    /**
     * Singleton registry of all Ops
    */
    class OpRegistry
    {
    public:
        /**
         * @returns OpRegistry singleton instance
        */
        static OpRegistry& instance();

        /**
         * @param name Name of op to get. Case sensitive
         * @returns Op with matching name
        */
        const Op& GetOp(const std::string& name) const;
        bool HasOp(const std::string& name) const;
        size_t size() const;

    private:
        friend class OpKernelBuilder;
        friend class OpBuilder;

        /**
         * Register an op to registry
         * 
         * @param op Op to be registered
         * @returns Register status
        */
        Status RegisterOp(const Op&& op);

        /**
         * Register an OpKernel to Op in registry
         * 
         * @param op_name Name of target op
         * @param kernel The kernel definition to be registered
         * @returns Register status
        */
        Status RegisterOpKernel(const std::string& op_name, OpKernelDef&& kernel);

        OpRegistry() = default;
        OpRegistry(const OpRegistry&)               = delete;
        OpRegistry& operator=(const OpRegistry&)    = delete;

        std::unordered_map<std::string, Op> ops_;
    };


    /**
     * Builds a OpKernelDef which defines the attributes of
     * an OpKernel instance
     * 
     * @param T type derived from OpKernel
    */
    template <typename T>
    class OpKernelDefBuilder
    {
    public:

        /**
         * Adds an input tensor
         * 
         * @param dtype Data type of input tensor
         * @returns This builder
        */
        OpKernelDefBuilder& Input(DataType dtype)
        {
            input_dtypes_.push_back(dtype);
        }

        /**
         * Adds an output tensor
         * 
         * @param dtype Data type of output tensor
         * @returns This builder
        */
        OpKernelDefBuilder& Output(DataType dtype)
        {
            input_dtypes_.push_back(dtype);
        }
        
        /**
         * Finalize and build OpKernelDef into Op
         * 
         * @returns Initializer object to allow out-of-main initalization
        */
        Initializer Build() const
        {
            OpKernelDef kernel(device_, create_fn_, input_dtypes_, output_dtypes_);
            Status status = OpRegistry::instance().RegisterOpKernel(target_op_name_, std::move(kernel));
            GL_CHECK_OK(status);
            return Initializer();
        }
        
        /**
         * @param target_op_name Name of the op the kernel is registered to
         * @param device Device type this kernel runs on
        */
        OpKernelDefBuilder(const std::string& target_op_name, 
            const std::string& device) : 
            target_op_name_(target_op_name),
            device_(device),
            create_fn_([](const OpKernelContext& context) {return new T(context);})
        {
            if (!std::is_base_of<OpKernel, T>::value)
            {
                throw GlException("Can only build with OpKernel derived type");
            }
        }

    private:
        std::vector<DataType> input_dtypes_; 
        std::vector<DataType> output_dtypes_;
        std::function<OpKernel*(const OpKernelContext&)> create_fn_; // lambda function that creates the OpKernel instance
        const std::string target_op_name_;
        const std::string device_;
    };
}


#endif