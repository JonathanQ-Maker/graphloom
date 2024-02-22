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
    try
    {
        OpBuilder(op_name).
            Input().
            Input().
            Attribute("A1").
            Attribute("A1"). // expected error
            Output([](const ComputeContext& c, LayoutArray& shape){
                shape = {3, 5};
                return Status::kOK;
            }).Build();
        FAIL() << "Expected GL_REGISTER_OP to throw an expection, but none recived.";
    }
    catch(const GlException& e)
    {
        // success
        EXPECT_FALSE(OpRegistry::instance().HasOp(op_name))
            << "Expected op to fail to register, instead op exists in registry";
    }
}

TEST(RegisterOpSuite, DuplicateOpName)
{
    std::string op_name("test_op");
    try
    {
        OpBuilder(op_name).Build();
        FAIL() << "Expected GL_REGISTER_OP to throw an expection, but none recived.";
    }
    catch(const GlException& e)
    {
        // success
    }
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}