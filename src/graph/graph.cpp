#include "graph/graph.h"

namespace graphloom
{
    /**
     * Graph Impl
    */
    void Graph::Clear()
    {
        for (const Node* node : nodes_) {
            delete node;
        }

        for (const Edge* edge : edges_) {
            delete edge;
        }

        nodes_.clear();
        edges_.clear();
        attributes_.clear();
    }

    Graph::~Graph()
    {
        Clear();
    }

    Graph::Graph(Graph&& other)
    {
        if (this != &other)
        {
            nodes_ = std::move(other.nodes_);
            edges_ = std::move(other.edges_);
            attributes_ = std::move(other.attributes_);
        }
    }

    Graph& Graph::operator=(Graph&& other)
    {
        if (this != &other)
        {
            nodes_ = std::move(other.nodes_);
            edges_ = std::move(other.edges_);
            attributes_ = std::move(other.attributes_);
        }
        return *this;
    }
    
    bool Graph::IsValidNode(const Node* node) const
    {
        if (node == nullptr) return false;
        if (node->id() < 0) return false;
        return node == nodes_[node->id()];
    }
    
    bool Graph::HasAttr(const std::string& path) const
    {
        return attributes_.find(path) != attributes_.end();
    }

    int32_t Graph::GetInt32Attr(const std::string& path) const
    {
        return std::any_cast<int32_t>(attributes_.at(path));
    }

    int64_t Graph::GetInt64Attr(const std::string& path) const
    {
        return std::any_cast<int64_t>(attributes_.at(path));
    }

    float Graph::GetFloatAttr(const std::string& path) const
    {
        return std::any_cast<float>(attributes_.at(path));
    }

    double Graph::GetDoubleAttr(const std::string& path) const
    {
        return std::any_cast<double>(attributes_.at(path));
    }

    bool Graph::GetBoolAttr(const std::string& path) const
    {
        return std::any_cast<bool>(attributes_.at(path));
    }


    /**
     * Node impl
    */
    Node::~Node()
    {
        delete kernel_;
        kernel_ = nullptr;
    }

    std::string Node::name() const
    {
        return name_;
    }

    Status Node::Compute(ComputeContext& context)
    {
        return kernel_->Compute(context);
    }

    int Node::id() const
    {
        return id_;
    }

    Node::Node(const std::string& name, int id) : 
        name_(name), id_(id)
    {

    }


    /**
     * Edge Impl
    */
    Node* Edge::src() const
    {
        return src_;
    }

    Node* Edge::dest() const
    {
        return dest_;
    }

    size_t Edge::src_id() const
    {
        return src_id_;
    }


    size_t Edge::dest_id() const
    {
        return dest_id_;
    }

}