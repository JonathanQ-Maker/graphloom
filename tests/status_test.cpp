#include <gtest/gtest.h>
#include <graphloom/graphloom.h>
#include <string>
#include <utility>
#include <sstream>

using namespace graphloom;

TEST(StatusSuite, ConstructTest)
{
    int age = 37;
    int code = 378;
    std::string name("John");
    bool hasDog = true;

    Status status(code, "The person has name \"", name, "\", age ", age, ". hasDog=", hasDog);

    std::ostringstream oss;
    oss << "The person has name \"";
    oss << name;
    oss << "\", age ";
    oss <<age;
    oss << ". hasDog=";
    oss << hasDog;

    EXPECT_EQ(status.msg(), oss.str());
    EXPECT_EQ(status.code(), 378);
    EXPECT_FALSE(status.ok());
}

TEST(StatusSuite, CopyTest)
{
    int age     = 37;
    int code    = 378;
    std::string name("John");
    bool hasDog = true;

    Status status(code, "The person has name \"", name, "\", age ", age, ". hasDog=", hasDog);

    Status copy(status);

    EXPECT_EQ(status.msg(), copy.msg());
    EXPECT_EQ(status.code(), copy.code());
}

TEST(StatusSuite, AssignTest)
{
    int age     = 37;
    int code    = 378;
    std::string name("John");
    bool hasDog = true;

    Status status(code, "The person has name \"", name, "\", age ", age, ". hasDog=", hasDog);

    Status copy = status;

    EXPECT_EQ(status.msg(), copy.msg());
    EXPECT_EQ(status.code(), copy.code());
}

TEST(StatusSuite, MoveConstructTest)
{
    int age     = 37;
    int code    = 378;
    std::string name("John");
    bool hasDog = true;

    Status status(code, "The person has name \"", name, "\", age ", age, ". hasDog=", hasDog);
    std::string msg = status.msg();
    Status copy(std::move(status));

    EXPECT_TRUE(status.msg().empty());
    EXPECT_EQ(status.code(), 0);

    EXPECT_EQ(copy.msg(), msg);
}

TEST(StatusSuite, MoveAssignTest)
{
    int age     = 37;
    int code    = 378;
    std::string name("John");
    bool hasDog = true;

    Status status(code, "The person has name \"", name, "\", age ", age, ". hasDog=", hasDog);
    std::string msg = status.msg();
    Status copy = std::move(status);

    EXPECT_TRUE(status.msg().empty());
    EXPECT_EQ(status.code(), 0);

    EXPECT_EQ(copy.msg(), msg);
}

TEST(StatusSuite, StatuskOK)
{
    Status status = Status::kOK;
    EXPECT_TRUE(status.ok());
    EXPECT_EQ(status.msg(), "OK");
    EXPECT_EQ(status.code(), 0);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}