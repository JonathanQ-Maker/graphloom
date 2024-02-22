#ifndef GRAPHLOOM_GRAPH__GRAPH_FACTORY_H_
#define GRAPHLOOM_GRAPH__GRAPH_FACTORY_H_

#include "graphloom/graph/graph_def.h"
#include "graphloom/common/status.h"

#include "graph/graph.h"

namespace graphloom
{
    class GraphFactory
    {
    public:
        static Status UpdateGraph(const GraphDef& graph_def, Graph& graph);
    private:
        static Node* CreateNode(const NodeDef* node_def);
        static Edge* CreateEdge(const EdgeDef* edge_def);

        static Status ResolveKernel(const NodeDef* node_def, 
            std::function<OpKernel*(const OpKernelContext&)>& result);
    };
}

#endif