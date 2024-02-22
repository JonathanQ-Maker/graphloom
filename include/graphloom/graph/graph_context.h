#ifndef GRAPHLOOM_GRAPH_GRAPH_CONTEXT_H_
#define GRAPHLOOM_GRAPH_GRAPH_CONTEXT_H_

#include <string>

/**
 * This module defines the contexts used by 
 * internal computational graph during execution.
*/

namespace graphloom
{
    class NodeDef;
    
    /**
     * OpKernel's construction context.
     * Passed to OpKernel's constructor.
    */
    class OpKernelContext
    {
    public:
        /**
         * Returns the attribute at path
         * 
         * @param path relative path
         * @returns attribute
        */
        int32_t GetInt32Attr(const std::string& path) const;

        /**
         * Returns the attribute at path
         * 
         * @param path relative path
         * @returns attribute
        */
        int64_t GetInt64Attr(const std::string& path) const;

        /**
         * Returns the attribute at path
         * 
         * @param path relative path
         * @returns attribute
        */
        float GetFloatAttr(const std::string& path) const;

        /**
         * Returns the attribute at path
         * 
         * @param path relative path
         * @returns attribute
        */
        double GetDoubleAttr(const std::string& path) const;

        /**
         * Returns the attribute at path
         * 
         * @param path relative path
         * @returns attribute
        */
        bool GetBoolAttr(const std::string& path) const;

    private:
        friend class GraphFactory;

        /**
         * @param node_def the NodeDef that carries the attributes
        */
        OpKernelContext(const NodeDef* node_def);

        const NodeDef* node_def_; // NodeDef that carries the attributes
    };

    /**
     * OpKernel's compute context. Passed to 
     * OpKernel's Compute() during exectuion
    */
    class ComputeContext
    {
    public:
        // TODO: fill
    };
}

#endif