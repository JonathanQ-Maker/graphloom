#ifndef GRAPHLOOM_GRAPH_NODE_DEF_BUILDER_H_
#define GRAPHLOOM_GRAPH_NODE_DEF_BUILDER_H_

#include <vector>
#include <unordered_map>
#include <any>
#include <utility>
#include <string>
#include <cstdint>
#include <initializer_list>

#include "graphloom/graph/graph_def.h"
#include "graphloom/common/status.h"
#include "graphloom/common/data_type.h"

/**
 * This module defines the NodeDefBuilder which
 * is the interface a user must use to build the 
 * GraphDef. Each NodeDefBuilder is intended to
 * build a single node.
*/

namespace graphloom
{
    /**
     * Builder that step by step builds a NodeDef into GraphDef.
    */
    class NodeDefBuilder
    {
    public:

        /**
         * Add an input node for this node.
         * 
         * @param src_node The input node. Required to be from the same graph
         * @param src_id Tndex/id of output to connect to this node
         * @returns This builder
        */
        NodeDefBuilder& Input(NodeDef* src_node, size_t src_id);

        /**
         * Set the name of this node.
         * 
         * @param name New name
         * @returns This builder
        */
        NodeDefBuilder& Name(const std::string& name);

        /**
         * Sets the attribute at path
         * 
         * @param path Relative path
         * @param value Attribute
         * @returns This builder
        */
        NodeDefBuilder& SetAttr(const std::string& name, int32_t value);

        /**
         * Sets the attribute at path
         * 
         * @param path Relative path
         * @param value Attribute
         * @returns This builder
        */
        NodeDefBuilder& SetAttr(const std::string& name, int64_t value);

        /**
         * Sets the attribute at path
         * 
         * @param path Relative path
         * @param value Attribute
         * @returns This builder
        */
        NodeDefBuilder& SetAttr(const std::string& name, float value);

        /**
         * Sets the attribute at path
         * 
         * @param path Relative path
         * @param value Attribute
         * @returns This builder
        */
        NodeDefBuilder& SetAttr(const std::string& name, double value);

        /**
         * Sets the attribute at path
         * 
         * @param path Relative path
         * @param value Attribute
         * @returns This builder
        */
        NodeDefBuilder& SetAttr(const std::string& name, bool value);

        /**
         * Finalize and build the node into the graph.
         * 
         * @param dtypes Sets the output data types. 
         * The builder uses this to resolve an op kernel
         * @returns NodeRef to the created node
        */
        NodeDef* Build(const std::initializer_list<DataType>& dtypes);

        /**
         * Reset the builder configurations except 
         * for parameters defined in constructor 
        */
        void Reset();

        /**
         * @param graph The graph to build the node into
         * @param op_name Name of the op this node represents
         * @param device Name of the device the op runs on 
        */
        NodeDefBuilder(GraphDef& graph, 
            const std::string& op_name, 
            const std::string& device);

    private:

        /**
         * Helper function to add an edge between src and dest nodes.
         * 
         * @param src Source node
         * @param src_id Index/id of the output from the source node
         * @param dest Destination node
         * @param dest_id Index/id of the input to the destination node
        */
        void AddEdge(NodeDef* src, size_t src_id, 
            NodeDef* dest, size_t dest_id);

        /**
         * Helper function to add a node to the graph
         * 
         * @param node Node to add
        */
        void AddNode(NodeDef* node);

        /**
         * Generates a uniqe node name in the graph from a base name.
         * 
         * Given base "myNode" possible names:
         * myNode_1,
         * myNode_2,
         * myNode_3 ...
         * 
         * @param base_name Name to base on
         * @returns Unique node name in this graph
        */
        std::string GenUniqueNodeName(const std::string& base_name);

        std::vector<std::pair<NodeDef*, size_t>> node_inputs_;
        std::unordered_map<std::string, std::any> attributes_;
        std::string name_;
        std::string device_;
        const Op& op_;
        GraphDef& graph_;
    };
}

#endif