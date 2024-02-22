#include <gtest/gtest.h>
#include <graphloom/graphloom.h>

#include <string>

using namespace graphloom;

GL_REGISTER_OP("test_op").
    Input().
    Input().
    Attribute("A1").
    Attribute("A2").
    Output([](const ComputeContext& c, LayoutArray& shape){
        shape = {3, 5};
        return Status::kOK;
    }).Build();

TEST(RegisterOpSuite, ValidTestOp)
{
    std::string op_name("test_op");

    EXPECT_GE(OpRegistry::instance().size(), 1)
        << "Expected at least 1 op, but got 0";
    EXPECT_TRUE(OpRegistry::instance().HasOp(op_name))
        << "Expected the op \"" << op_name << "\" to be in registry, but got false";
    
    const Op& op = OpRegistry::instance().GetOp(op_name);
    
    EXPECT_EQ(op.name(), op_name)
        << "Name to get the op from registry and name reported by op itself is mismatched.";

    auto attributes  = op.attributes();
    EXPECT_EQ(attributes.size(), 2);
    EXPECT_NE(attributes.find("A1"), attributes.end())
        << "Expcted the attibute \"A1\" to exist, but is not found";
    EXPECT_NE(attributes.find("A2"), attributes.end())
        << "Expcted the attibute \"A2\" to exist, but is not found";

    EXPECT_EQ(op.num_inputs(), 2);
    EXPECT_EQ(op.num_outputs(), 1);
}

TEST(RegisterOpSuite, InvalidAttr)
{
    std::string op_name("test_op2");

    EXPECT_THROW(OpBuilder(op_name).
            Input().
            Input().
            Attribute("A1").
            Attribute("A1"). // expected error
            Output([](const ComputeContext& c, LayoutArray& shape){
                shape = {3, 5};
                return Status::kOK;
            }).Build(), GlException);

    EXPECT_FALSE(OpRegistry::instance().HasOp(op_name))
        << "Expected op to fail to register, instead op exists in registry";
}

TEST(RegisterOpSuite, DuplicateOpName)
{
    // Should fail and throw due to duplicate name
    EXPECT_THROW(OpBuilder("test_op").Build(), GlException);    
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}