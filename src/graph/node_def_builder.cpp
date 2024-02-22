#include "graphloom/graph/node_def_builder.h"
#include "graphloom/op/registration.h"
#include "graphloom/device/registration.h"

namespace graphloom
{
    NodeDefBuilder& NodeDefBuilder::Input(NodeDef* src_node, size_t src_id)
    {
        if (!graph_.IsValidNode(src_node))
        {
            throw GlException("Source node is not valid in this GraphDef");
        }

        const Op& src_op = src_node->op();
        if (src_id >= src_op.num_outputs())
        {
            throw GlException("Source node \"", src_node->name(), "\" only has ", 
                            src_op.num_outputs(), 
                            " outputs, src_id=", src_id);
        }

        if (node_inputs_.size() >= op_.num_inputs())
        {
            throw GlException("Op \"", op_.name(), "\" only has ", 
                            op_.num_inputs(), " inputs");
        }

        node_inputs_.push_back({src_node, src_id});
        return *this;
    }

    NodeDefBuilder& NodeDefBuilder::Name(const std::string& name)
    {
        name_ = name;
        return *this;
    }

    NodeDefBuilder& NodeDefBuilder::SetAttr(const std::string& name, int32_t value)
    {
        attributes_[name] = value;
        return *this;
    }

    NodeDefBuilder& NodeDefBuilder::SetAttr(const std::string& name, int64_t value)
    {
        attributes_[name] = value;
        return *this;
    }

    NodeDefBuilder& NodeDefBuilder::SetAttr(const std::string& name, float value)
    {
        attributes_[name] = value;
        return *this;
    }

    NodeDefBuilder& NodeDefBuilder::SetAttr(const std::string& name, double value)
    {
        attributes_[name] = value;
        return *this;
    }

    NodeDefBuilder& NodeDefBuilder::SetAttr(const std::string& name, bool value)
    {
        attributes_[name] = value;
        return *this;
    }

    NodeDef* NodeDefBuilder::Build(const std::initializer_list<DataType>& dtypes)
    {
        if (dtypes.size() != op_.num_outputs())
        {
            throw GlException("Mismatched number of output DataTypes. \"", op_.name(), 
                "\" requires ", op_.num_outputs(), " outputs");
        }

        if (node_inputs_.size() != op_.num_inputs())
        {
            throw GlException("Mismatched number of inputs. \"", op_.name(), 
                "\" requires ", op_.num_inputs(), " inputs");
        }

        if (op_.attributes().size() != attributes_.size())
        {
            throw GlException("Mismatched number of attributes required by the op and attributes given.");
        }

        for (const std::string& attr : op_.attributes())
        {
            if (attributes_.find(attr) == attributes_.end())
            {
                throw GlException("Required attribute \"", attr, "\" missing");
            }
        }

        std::string name = GenUniqueNodeName(name_);
        NodeDef* node = new NodeDef(op_, name, device_);
        AddNode(node);
        node->out_dtypes_ = std::move(dtypes);

        // build input edges
        for (int i = 0; i < op_.num_inputs(); ++i)
        {
            NodeDef* src     = node_inputs_[i].first;
            size_t src_id    = node_inputs_[i].second;

            AddEdge(src, src_id, node, i);
        }
        
        // build attributes
        for (auto pair : attributes_)
        {
            std::string path(name);
            path += "/";
            path += pair.first;

            graph_.attributes_[path] = pair.second;
        }
        

        return node;
    }

    void NodeDefBuilder::Reset()
    {
        node_inputs_.clear();
        attributes_.clear();
        name_.clear();
    }

    NodeDefBuilder::NodeDefBuilder(GraphDef& graph, 
        const std::string& op_name, 
        const std::string& device) : 
        op_(OpRegistry::instance().GetOp(op_name)),
        graph_(graph)
    {
        if (!DeviceRegistry::instance().HasDevice(device_))
        {
            throw GlException("Requested device \"", device_, "\" does not exist");
        }
    }

    void NodeDefBuilder::AddEdge(NodeDef* src, size_t src_id, 
        NodeDef* dest, size_t dest_id)
    {
        EdgeDef* edge = new EdgeDef(src, src_id, dest, dest_id);
        graph_.edges_.insert(edge);

        src->out_edges_.push_back(edge);
        dest->in_edges_.push_back(edge);
    }

    void NodeDefBuilder::AddNode(NodeDef* node)
    {
        node->id_ = graph_.nodes_.size();
        graph_.nodes_.push_back(node);
    }

    std::string NodeDefBuilder::GenUniqueNodeName(const std::string& base_name)
    {
        std::string name(base_name);
        for (int i = 1; graph_.FindNode(name); ++i)
        {
            name = base_name;
            name += "_";
            name += i;
        }
        return name;
    }
}