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

TEST(NodeDefBuilderSuite, ValidBuildTest)
{
    GraphDef graph;
    NodeDef* node = NodeDefBuilder(graph, "test_op", "CPU:0").
        SetAttr("A1", 3).
        SetAttr("A2", 1).
        Name("myNode").
        Build({DataType::Float});

    ASSERT_TRUE(node);
    EXPECT_EQ(node->name(), "myNode");
    EXPECT_EQ(node->device(), "CPU:0");
    EXPECT_TRUE(graph.IsValidNode(node));

    const std::vector<graphloom::DataType>& out_dtypes = node->out_dtypes();
    EXPECT_EQ(out_dtypes.size(), 1);
    EXPECT_EQ(out_dtypes[0], DataType::Float);
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
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}