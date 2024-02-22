#ifndef GRAPHLOOM_GRAPH_GRAPH_DEF_H_
#define GRAPHLOOM_GRAPH_GRAPH_DEF_H_

#include <unordered_set>
#include <unordered_map>
#include <any>
#include <vector>
#include <string>
#include <cstdint>

#include "graphloom/common/status.h"
#include "graphloom/op/op.h"
#include "graphloom/common/data_type.h"
#include "graphloom/device/device.h"

/**
 * This module defines the GraphDef and closely related 
 * classes. GraphDef does not compute anything, 
 * rather, acts as a intermediate representation 
 * such that the graph can be used in downstream
 * tasks. Example tasks include serialization, 
 * optimization, execution and human visual rendering. 
*/

namespace graphloom
{
    class EdgeDef;
    class NodeDef;
    class GraphDef;
    class GraphFactory;

    /**
     * Computational graph intermediate 
     * representation.
     * 
     * Contains the nodes, edges and 
     * attributes that define a graph
    */
    class GraphDef
    {
    public:
        GraphDef() = default;
        ~GraphDef();

        GraphDef(const GraphDef&)               = delete;
        GraphDef& operator=(const GraphDef&)    = delete;
        GraphDef(GraphDef&&)                    = delete;
        GraphDef& operator=(GraphDef&&)         = delete;
        
        /**
         * Check if this graph have Attribute
         * 
         * @param path Absolute path
         * @returns Attribute existance
        */
        bool HasAttr(const std::string& path) const;

        /**
         * @returns Number of nodes in this graph
        */
        size_t num_nodes() const;

        /**
         * Search for a node. O(n) complexity
         * 
         * @param name Node's name, case-sensitive
         * @returns The node if found, nullptr otherwise 
        */
        NodeDef* FindNode(const std::string& name) const;

        /**
         * Checks if node is a "valid" node.
         * 
         * A node is valid if:
         *  not nullptr
         *  exists in this graph
         * 
         * @param node Node to check
         * @returns Node validity
        */
        bool IsValidNode(const NodeDef* node) const;

        /**
         * Returns the Attribute at path
         * 
         * @param path Absolute path
         * @returns Attribute
        */
        int32_t GetInt32Attr(const std::string& path) const;

        /**
         * Returns the Attribute at path
         * 
         * @param path Absolute path
         * @returns Attribute
        */
        int64_t GetInt64Attr(const std::string& path) const;

        /**
         * Returns the Attribute at path
         * 
         * @param path Absolute path
         * @returns Attribute
        */
        float GetFloatAttr(const std::string& path) const;

        /**
         * Returns the Attribute at path
         * 
         * @param path Absolute path
         * @returns Attribute
        */
        double GetDoubleAttr(const std::string& path) const;

        /**
         * Returns the Attribute at path
         * 
         * @param path Absolute path
         * @returns Attribute
        */
        bool GetBoolAttr(const std::string& path) const;

    private:
        friend class NodeDefBuilder;
        friend class GraphFactory;
        
        std::vector<NodeDef*> nodes_; 
        std::unordered_set<EdgeDef*> edges_;
        
        // Map of absolute path to attributes
        std::unordered_map<std::string, std::any> attributes_; 
    };

    /**
     * GraphDef's node class. Each node 
     * represents an operation in the graph.
    */
    class NodeDef
    {
    public:

        /**
         * @returns Node's name
        */
        std::string name() const;

        /**
         * @returns The operation this node represents
        */
        const Op& op() const;

        /**
         * @returns Name of device to execute op on request by user 
        */
        const std::string& device() const;

        /**
         * @returns Node's id in the graph
        */
        int id() const;

        /**
         * @returns The op's output types requested by user
        */
        const std::vector<DataType>& out_dtypes() const;

    private:
        friend class NodeDefBuilder;
        friend class GraphDef;
        friend class GraphFactory;
        
        /**
         * @param op The operation this node represents
         * @param name Name of this node
         * @param device Name of device to execute op on request by user 
        */
        NodeDef(const Op& op, const std::string& name, const std::string& device);
        ~NodeDef() = default;
        
        int id_;
        const Op& op_;
        std::string name_;
        std::string device_;
        std::vector<EdgeDef*> in_edges_;
        std::vector<EdgeDef*> out_edges_;
        std::vector<DataType> out_dtypes_;
    };

    class EdgeDef
    {
    public:
        /**
         * @returns Source node
        */
        NodeDef* src() const;

        /**
         * @returns Destination node
        */
        NodeDef* dest() const;

        /**
         * @returns Index/id of the output from the source node
        */
        size_t src_id() const;

        /**
         * @returns Index/id of the input to the destination node
        */
        size_t dest_id() const;

    private:
        friend class NodeDefBuilder;
        
        /**
         * @param src Source node
         * @param src_id Index/id of the output from the source node
         * @param dest Destination node
         * @param dest_id Index/id of the input to the destination node
        */
        EdgeDef(NodeDef* src, size_t src_id, 
            NodeDef* dest, size_t dest_id);

        NodeDef* src_;
        NodeDef* dest_;
        size_t src_id_;
        size_t dest_id_;
    };
}

#endif