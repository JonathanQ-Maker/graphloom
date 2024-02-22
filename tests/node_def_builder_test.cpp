#include <gtest/gtest.h>
#include <graphloom/graphloom.h>

using namespace graphloom;

GL_REGISTER_OP("test_op").
    Attribute("A1").
    Attribute("A2").
    Output([](const ComputeContext& c, LayoutArray& shape){
        shape = {3, 1};
        return Status::kOK;
    }).
    Build();

GL_REGISTER_OP("test_input_op").
    Attribute("B1").
    Input().
    Output([](const ComputeContext& c, LayoutArray& shape){
        shape = {3, 1};
        return Status::kOK;
    }).
    Build();

TEST(NodeDefBuilderSuite, ValidBuildTest)
{
    GraphDef graph;
    NodeDef* node = NodeDefBuilder(graph, "test_op", "CPU:0").
        SetAttr("A1", 3).
        SetAttr("A2", true).
        Name("myNode").
        Build({DataType::Float});

    ASSERT_TRUE(node);
    EXPECT_EQ(node->name(), "myNode");
    EXPECT_EQ(node->device(), "CPU:0");
    EXPECT_EQ(node->id(), 0);

    EXPECT_TRUE(graph.IsValidNode(node));
    EXPECT_EQ(graph.num_nodes(), 1);
    EXPECT_EQ(graph.GetInt32Attr("myNode/A1"), 3);
    EXPECT_EQ(graph.GetBoolAttr("myNode/A2"), true);
    EXPECT_TRUE(graph.HasAttr("myNode/A1"));
    EXPECT_TRUE(graph.HasAttr("myNode/A2"));

    const std::vector<graphloom::DataType>& out_dtypes = node->out_dtypes();
    EXPECT_EQ(out_dtypes.size(), 1);
    EXPECT_EQ(out_dtypes[0], DataType::Float);

    EXPECT_ANY_THROW(graph.GetDoubleAttr("myNode/A1"));
}

TEST(NodeDefBuilderSuite, InvalidOpName)
{
    GraphDef graph;
    NodeDef* node = nullptr;
    EXPECT_ANY_THROW(node = NodeDefBuilder(graph, "invalid_op_name", "CPU:0").
        SetAttr("A1", 3).
        SetAttr("A2", 1).
        Name("myNode").
        Build({DataType::Float}));

    EXPECT_FALSE(node);
}

TEST(NodeDefBuilderSuite, InvalidDeviceName)
{
    GraphDef graph;
    NodeDef* node = nullptr;
    EXPECT_ANY_THROW(node = NodeDefBuilder(graph, "test_op", "invalid_device_name").
        SetAttr("A1", 3).
        SetAttr("A2", 1).
        Name("myNode").
        Build({DataType::Float}));

    EXPECT_FALSE(node);
    EXPECT_FALSE(graph.HasAttr("myNode/A1"));
    EXPECT_FALSE(graph.HasAttr("myNode/A2"));
}

TEST(NodeDefBuilderSuite, InvalidAttrName)
{
    GraphDef graph;
    NodeDef* node = nullptr;
    EXPECT_ANY_THROW(node = NodeDefBuilder(graph, "test_op", "CPU:0").
        SetAttr("invalid_attr_name", 3).
        SetAttr("A2", 1).
        Name("myNode").
        Build({DataType::Float}));

    EXPECT_FALSE(node);
    EXPECT_FALSE(graph.HasAttr("myNode/invalid_attr_name"));
    EXPECT_FALSE(graph.HasAttr("myNode/A2"));
}

TEST(NodeDefBuilderSuite, InvalidOutDtypes)
{
    GraphDef graph;
    NodeDef* node = nullptr;
    EXPECT_ANY_THROW(node = NodeDefBuilder(graph, "test_op", "CPU:0").
        SetAttr("A1", 3).
        SetAttr("A2", 1).
        Name("myNode").
        Build({}));

    EXPECT_FALSE(node);
    EXPECT_FALSE(graph.HasAttr("myNode/A1"));
    EXPECT_FALSE(graph.HasAttr("myNode/A2"));
}

TEST(NodeDefBuilderSuite, TwoNodesTest)
{
    GraphDef graph;
    NodeDef* node0 = NodeDefBuilder(graph, "test_op", "CPU:0").
        SetAttr("A1", 3).
        SetAttr("A2", true).
        Name("myNode").
        Build({DataType::Float});

    NodeDef* node1 = NodeDefBuilder(graph, "test_input_op", "CPU:0").
        Input(node0, 0).
        SetAttr("B1", -8).
        Name("myNode").
        Build({DataType::Int16});

    // Node0 checks
    ASSERT_TRUE(node0);
    EXPECT_EQ(node0->name(), "myNode");
    EXPECT_EQ(node0->device(), "CPU:0");
    EXPECT_EQ(node0->id(), 0);

    EXPECT_TRUE(graph.IsValidNode(node0));
    EXPECT_EQ(graph.GetInt32Attr("myNode/A1"), 3);
    EXPECT_EQ(graph.GetBoolAttr("myNode/A2"), true);
    EXPECT_TRUE(graph.HasAttr("myNode/A1"));
    EXPECT_TRUE(graph.HasAttr("myNode/A2"));

    const std::vector<graphloom::DataType>& out_dtypes0 = node0->out_dtypes();
    EXPECT_EQ(out_dtypes0.size(), 1);
    EXPECT_EQ(out_dtypes0[0], DataType::Float);


    // Node1 checks
    ASSERT_TRUE(node1);
    EXPECT_EQ(node1->name(), "myNode_1");
    EXPECT_EQ(node1->device(), "CPU:0");
    EXPECT_EQ(node1->id(), 1);

    EXPECT_TRUE(graph.IsValidNode(node1));
    EXPECT_EQ(graph.num_nodes(), 2);
    EXPECT_EQ(graph.GetInt32Attr("myNode_1/B1"), -8);
    EXPECT_TRUE(graph.HasAttr("myNode_1/B1"));

    const std::vector<graphloom::DataType>& out_dtypes1 = node1->out_dtypes();
    EXPECT_EQ(out_dtypes1.size(), 1);
    EXPECT_EQ(out_dtypes1[0], DataType::Int16);
}

TEST(NodeDefBuilderSuite, TwoNodesDiffGraphTest)
{
    GraphDef graph0;
    GraphDef graph1;
    NodeDef* node0 = NodeDefBuilder(graph0, "test_op", "CPU:0").
        SetAttr("A1", 3).
        SetAttr("A2", true).
        Name("myNode").
        Build({DataType::Float});

    NodeDef* node1 = nullptr;
    EXPECT_ANY_THROW(node1 = NodeDefBuilder(graph1, "test_input_op", "CPU:0").
        Input(node0, 0).
        SetAttr("B1", -8).
        Name("myNode").
        Build({DataType::Int16}));

    EXPECT_FALSE(node1);
    EXPECT_EQ(graph0.num_nodes(), 1);
    EXPECT_EQ(graph1.num_nodes(), 0);
}

TEST(NodeDefBuilderSuite, InvalidInputNodeTest)
{
    GraphDef graph;
    NodeDef* node0 = nullptr;
    NodeDef* node1 = nullptr;

    EXPECT_ANY_THROW(node1 = NodeDefBuilder(graph, "test_input_op", "CPU:0").
        Input(node0, 0).
        SetAttr("B1", -8).
        Name("myNode").
        Build({DataType::Int16}));

    EXPECT_FALSE(node1);

    EXPECT_EQ(graph.num_nodes(), 0);
    EXPECT_ANY_THROW(graph.GetInt32Attr("myNode/B1"));
    EXPECT_FALSE(graph.HasAttr("myNode/B1"));
}

TEST(NodeDefBuilderSuite, InvalidInputNodeIdTest)
{
    GraphDef graph;
    NodeDef* node0 = NodeDefBuilder(graph, "test_op", "CPU:0").
        SetAttr("A1", 3).
        SetAttr("A2", true).
        Name("myNode").
        Build({DataType::Float});

    NodeDef* node1 = nullptr;
    EXPECT_ANY_THROW(node1 = NodeDefBuilder(graph, "test_input_op", "CPU:0").
        Input(node0, 999). // error input index
        SetAttr("B1", -8).
        Name("myNode").
        Build({DataType::Int16}));

    // Node0 checks
    ASSERT_TRUE(node0);
    EXPECT_EQ(node0->name(), "myNode");
    EXPECT_EQ(node0->device(), "CPU:0");
    EXPECT_EQ(node0->id(), 0);

    EXPECT_TRUE(graph.IsValidNode(node0));
    EXPECT_EQ(graph.GetInt32Attr("myNode/A1"), 3);
    EXPECT_EQ(graph.GetBoolAttr("myNode/A2"), true);
    EXPECT_TRUE(graph.HasAttr("myNode/A1"));
    EXPECT_TRUE(graph.HasAttr("myNode/A2"));

    const std::vector<graphloom::DataType>& out_dtypes0 = node0->out_dtypes();
    EXPECT_EQ(out_dtypes0.size(), 1);
    EXPECT_EQ(out_dtypes0[0], DataType::Float);
    
    // Node1 checks
    EXPECT_FALSE(node1);

    EXPECT_EQ(graph.num_nodes(), 1);
    EXPECT_ANY_THROW(graph.GetInt32Attr("myNode_1/B1"));
    EXPECT_FALSE(graph.HasAttr("myNode_1/B1"));
}



int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}