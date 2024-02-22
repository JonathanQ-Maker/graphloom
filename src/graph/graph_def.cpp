#include <stdexcept>

#include "graphloom/graph/graph_def.h"


namespace graphloom
{
    /**
     * GraphDef impl
    */

    GraphDef::~GraphDef()
    {
        for (const NodeDef* node : nodes_) {
            delete node;
        }

        for (const EdgeDef* edge : edges_) {
            delete edge;
        }
        nodes_.clear();
        edges_.clear();
        attributes_.clear();
        source_nodes_.clear();
    }

    bool GraphDef::HasAttr(const std::string& path) const
    {
        return attributes_.find(path) != attributes_.end();
    }

    size_t GraphDef::num_nodes() const
    {
        return nodes_.size();
    }

    int32_t GraphDef::GetInt32Attr(const std::string& path) const
    {
        return std::any_cast<int32_t>(attributes_.at(path));
    }

    int64_t GraphDef::GetInt64Attr(const std::string& path) const
    {
        return std::any_cast<int64_t>(attributes_.at(path));
    }

    float GraphDef::GetFloatAttr(const std::string& path) const
    {
        return std::any_cast<float>(attributes_.at(path));
    }

    double GraphDef::GetDoubleAttr(const std::string& path) const
    {
        return std::any_cast<double>(attributes_.at(path));
    }

    bool GraphDef::GetBoolAttr(const std::string& path) const
    {
        return std::any_cast<bool>(attributes_.at(path));
    }

    NodeDef* GraphDef::FindNode(const std::string& name) const
    {
        for (NodeDef* node : nodes_) {
            if (node->name() == name)
            {
                return node;
            }
        }
        return nullptr;
    }

    bool GraphDef::IsValidNode(const NodeDef* node) const
    {
        if (node == nullptr) return false;
        if (node->id() < 0 || node->id() >= nodes_.size()) return false;
        return node == nodes_[node->id()];
    }


    /**
     * NodeDef impl
    */
   
    std::string NodeDef::name() const
    {
        return name_;
    }

    const Op& NodeDef::op() const
    {
        return op_;
    }

    const std::string& NodeDef::device() const
    {
        return device_;
    }

    int NodeDef::id() const
    {
        return id_;
    }

    const std::vector<DataType>& NodeDef::out_dtypes() const
    {
        return out_dtypes_;
    }

    NodeDef::NodeDef(const Op& op, const std::string& name, 
        const std::string& device) :
        op_(op),
        name_(name),
        device_(device)
    {

    }


    /**
     * EdgeDef impl
    */

    NodeDef* EdgeDef::src() const
    {
        return src_;
    }

    NodeDef* EdgeDef::dest() const
    {
        return dest_;
    }

    size_t EdgeDef::src_id() const
    {
        return src_id_;
    }

    size_t EdgeDef::dest_id() const
    {
        return dest_id_;
    }

    EdgeDef::EdgeDef(NodeDef* src, size_t src_id, 
        NodeDef* dest, size_t dest_id) :

        src_(src),
        src_id_(src_id),
        dest_(dest),
        dest_id_(dest_id)
    {
        
    }
}
