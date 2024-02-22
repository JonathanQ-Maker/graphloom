#include "graph/graph_factory.h"

namespace graphloom
{
    Status GraphFactory::UpdateGraph(const GraphDef& graph_def, Graph& graph)
    {
        graph.Clear();

        // copy attributes
        graph.attributes_ = graph_def.attributes_;

        // Fill nodes
        for (NodeDef* node_def : graph_def.nodes_)
        {
            graph.nodes_.push_back(CreateNode(node_def));
        }

        // Fill edges
        for (EdgeDef* edge_def : graph_def.edges_)
        {
            Node* src = graph.nodes_[edge_def->src()->id()];
            Node* dest = graph.nodes_[edge_def->dest()->id()];
            Edge* edge = new Edge(src, edge_def->src_id(), 
                                dest, edge_def->dest_id());
            graph.edges_.insert(edge);
        }

        return Status::kOK;
    }

    Node* GraphFactory::CreateNode(const NodeDef* node_def)
    {
        std::function<OpKernel*(const OpKernelContext&)> create_fn;
        GL_CHECK_OK(ResolveKernel(node_def, create_fn));

        Node* node = new Node(node_def->name(), node_def->id());
        node->kernel_ = create_fn(OpKernelContext(node_def));

        return node;
    }

    Status GraphFactory::ResolveKernel(const NodeDef* node_def, 
        std::function<OpKernel*(const OpKernelContext&)>& result)
    {
        const Op& op = node_def->op();

        for (const OpKernelDef& kernel_def : op.kernels_)
        {
            if (op.num_inputs() == 0)
            {
                // Special case. Node with no inputs, infer 
                // the kernel to use with output dtypes.

                // find matching kernel
                if (kernel_def.out_dtypes_ == node_def->out_dtypes())
                {
                    result = kernel_def.create_fn_;
                    return Status::kOK;
                }
            }
            else 
            {
                // Usual case. Infer kernel to use with input dtypes.

                // collect input data types because 
                // only output data types are kept 
                std::vector<DataType> input_dtypes;
                input_dtypes.reserve(op.num_inputs());
                for (EdgeDef* edge : node_def->in_edges_)
                {
                    input_dtypes.push_back(edge->src()->out_dtypes()[edge->src_id()]);
                }

                // find matching kernel
                if (kernel_def.in_dtypes_ == input_dtypes)
                {
                    result = kernel_def.create_fn_;
                    return Status::kOK;
                }
            }
        }

        return Status(1, "Failed to resolve OpKernel.");
    }
}