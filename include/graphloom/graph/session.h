#ifndef GRAPHLOOM_GRAPH_SESSION_H_
#define GRAPHLOOM_GRAPH_SESSION_H_

#include <initializer_list>
#include <vector>
#include <utility>

#include "graphloom/graph/graph_def.h"
#include "graphloom/graph/node_def_builder.h"
#include "graphloom/tensor/tensor.h"

namespace graphloom 
{
    class Graph;

    class Session
    {
    public:
        Session();
        ~Session();

        Session(const Session&)               = delete;
        Session& operator=(const Session&)    = delete;
        Session(Session&&)                    = delete;
        Session& operator=(Session&&)         = delete;

        void UpdateGraph(const GraphDef& graph);

        void Run(const std::vector<std::pair<NodeDef*, TensorBuffer*>>&, const std::vector<NodeDef>& target_nodes, std::vector<TensorBuffer>& outputs);

    private:
        Graph* const graph_;
    };
}

#endif