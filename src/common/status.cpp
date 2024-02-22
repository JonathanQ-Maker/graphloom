#include "graphloom/common/status.h"

namespace graphloom
{
    const Status Status::kOK(0, "OK");

    Status::Status(const Status& other) : 
        code_(other.code_), 
        msg_(other.msg_) 
    {

    }

    Status& Status::operator=(const Status& other) {
        if (this != &other) {
            code_ = other.code_;
            msg_ = other.msg_;
        }
        return *this;
    }

    Status::Status(Status&& other)
    {
        if (this != &other)
        {
            code_       = other.code_;
            other.code_ = 0;
            msg_        = std::move(other.msg_);
        }
    }

    Status& Status::operator=(Status&& other)
    {
        if (this != &other)
        {
            code_       = other.code_;
            other.code_ = 0;
            msg_        = std::move(other.msg_);
        }
        return *this;
    }

    std::string Status::msg() const
    {
        return msg_;
    }

    int Status::code() const
    { 
        return code_;
    }

    bool Status::ok() const
    {
        return !code_;
    }
}