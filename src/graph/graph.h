#ifndef GRAPHLOOM_GRAPH_GRAPH_H_
#define GRAPHLOOM_GRAPH_GRAPH_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <any>

#include "graphloom/op/op.h"
#include "graphloom/common/status.h"
#include "graphloom/graph/graph_context.h"

namespace graphloom
{
    class Edge;

    class Node
    {
    public:
        ~Node();

        std::string name() const;
        Status Compute(ComputeContext& context);
        int id() const;

    private:
        friend class GraphFactory;
        
        explicit Node(const std::string& name, int id);
        
        int id_;
        OpKernel* kernel_;
        std::string name_;
        std::vector<Edge*> in_edges_;
        std::vector<Edge*> out_edges_;
    };

    class Edge
    {
    public:
        Node* src() const;
        Node* dest() const;
        size_t src_id() const;
        size_t dest_id() const;

    private:
        friend class GraphFactory;

        Edge(Node* src, size_t src_id, Node* dest, size_t dest_id);

        Node* src_;
        Node* dest_;
        size_t src_id_;
        size_t dest_id_;
    };

    class Graph
    {
    public:
        void Clear();

        Graph() = default;
        ~Graph();

        Graph(const Graph&)               = delete;
        Graph& operator=(const Graph&)    = delete;
        Graph(Graph&& other);
        Graph& operator=(Graph&& other);
        
        bool IsValidNode(const Node* node) const;
        
        bool HasAttr(const std::string& path) const;

        int32_t GetInt32Attr(const std::string& path) const;
        int64_t GetInt64Attr(const std::string& path) const;
        float GetFloatAttr(const std::string& path) const;
        double GetDoubleAttr(const std::string& path) const;
        bool GetBoolAttr(const std::string& path) const;

    private:
        friend class GraphFactory;

        std::vector<Node*> nodes_;
        std::unordered_set<Edge*> edges_;
        std::unordered_map<std::string, std::any> attributes_;
    };
}

#endif