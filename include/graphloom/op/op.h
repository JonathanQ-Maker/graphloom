#ifndef GRAPHLOOM_OP_OP_H_
#define GRAPHLOOM_OP_OP_H_

#include <string>
#include <functional>
#include <unordered_map>
#include <set>
#include <vector>
#include <utility>


#include "graphloom/common/data_type.h"
#include "graphloom/tensor/tensor.h"
#include "graphloom/common/status.h"
#include "graphloom/device/device.h"
#include "graphloom/graph/graph_context.h"

/**
 * This module defines the operation and op kernel class
 * Op and OpKernel. An op represents an abstract idea
 * of an operation such as ADD and MULTIPLY. An op 
 * kernel, sometimes abriviated to just kernel, represents 
 * the concrete implementation to compute the op. 
 * 
 * For example, the ADD op can have 2 op kernels. One 
 * kernel uses the CPU and another uses CUDA to compute.
*/

namespace graphloom
{
    /**
     * Represents the concrete implemention to compute an Op.
     * 
     * OpKernel is intended to be inherited for specific kernel
     * implementations.
    */
    class OpKernel
    {
    public:
        /**
         * @param context the construction context of an op kernel
        */
        OpKernel(OpKernelContext& context);

        /**
         * Actually compute with the kernel
         * 
         * @param context the execution/compute context of the kernel
         * @returns execution status
        */
        virtual Status Compute(ComputeContext& context) = 0;
    };

    /**
     * Describes the attributes of an OpKernel.
    */
    class OpKernelDef
    {
    public:
        /**
         * @returns device type kernel executes on
        */
        std::string device() const;

        /**
         * @param id The index of input
         * @returns Input tensor type at index 
        */
        DataType input_dtype(size_t id) const;

        /**
         * @param id The index of output
         * @returns Output tensor type at index 
        */
        DataType output_dtype(size_t id) const;

        /**
         * @returns Number of input tensors
        */
        size_t num_inputs() const;

        /**
         * @returns Number of output tensors
        */
        size_t num_outputs() const;


    private:
        template<typename T>
        friend class OpKernelDefBuilder;
        friend class GraphFactory;

        /**
         * @param device Device type the OpKernel runs on
         * @param create_fn Function to create the OpKernel instance
         * @param in_dtypes Data types of the input tensors
         * @param out_dtypes Data types of the output tensors
        */
        OpKernelDef(const std::string& device, const std::function<OpKernel*(const OpKernelContext&)>& create_fn, 
            const std::vector<DataType>& in_dtypes, 
            const std::vector<DataType>& out_dtypes);

        const std::string device_;
        const std::function<OpKernel*(const OpKernelContext&)> create_fn_;
        const std::vector<DataType> in_dtypes_;
        const std::vector<DataType> out_dtypes_;
    };
    
    /**
     * Represents abstract idea of an operations.
    */
    class Op
    {
    public:

        /**
         * @returns Number of input tensors
        */
        size_t num_inputs() const;

        /**
         * @returns Number of output tensors
        */
        size_t num_outputs() const;

        /**
         * @returns Name of this op
        */
        std::string name() const;
        
        /**
         * @returns Set of required attributes for this op
        */
        const std::set<std::string>& attributes() const;

    private:
        friend class OpBuilder;
        friend class GraphFactory;
        friend class OpRegistry;

        Op() = default;

        std::set<std::string> attributes_;
        // list of functions that computes the shape of output 
        // tensor at their respective index
        std::vector<std::function<Status(const ComputeContext&, LayoutArray&)>> out_shape_fns_;
        std::vector<OpKernelDef> kernels_;
        size_t num_inputs_ = 0;
        size_t num_outputs_ = 0;
        std::string name_;
    };
}

#endif