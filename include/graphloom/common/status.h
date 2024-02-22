#ifndef GRAPHLOOM_COMMON_STATUS_H_
#define GRAPHLOOM_COMMON_STATUS_H_

#include <sstream>
#include <string>
#include <stdexcept>

/**
 * This file defines the "Status" module,
 * which allows the reporting of status 
 * messages and code to improve debug tracing.
*/


// throws GlException if status is not ok (code=0)
#define GL_CHECK_OK(x)                                      \
    do {                                                    \
        graphloom::Status result = (x);                     \
        if (!result.ok()) throw GlException(result.msg());  \
    } while (0)

namespace graphloom
{
    /**
     * A container class for a status report
    */
    class Status
    {
    public:

        // Short hand for Status(0, "OK")
        static const Status kOK;

        /**
         * Variadic template constructor that concatenates 
         * the status message.
         * 
         * @param code status code, ok == 0
         * @param args msg compoents to concatenate 
        */
        template<typename... Args>
        Status(int code, Args... args) : code_(code)
        {
            std::ostringstream oss;
            (oss << ... << args);
            msg_ = oss.str();
        }

        Status(const Status& other);
        Status& operator=(const Status& other);
        Status(Status&& other);
        Status& operator=(Status&& other);
        
        /**
         * Returns the status message
         * 
         * @returns status message
        */
        std::string msg() const;

        /**
         * Returns the status code
         * 
         * @returns status code
        */
        int code() const;

        /**
         * Returns true if ok
         * 
         * @returns code == 0
        */
        bool ok() const;

    private:
        int code_;
        std::string msg_;
    };

    // The base of all graphloom exceptions
    class GlException : public std::runtime_error 
    {
    public:

        /**
         * Variadic template constructor that formats the error message
         * 
         * @param args msg compoents to concatenate
        */
        template<typename... Args>
        GlException(Args... args)
            : std::runtime_error(createMessage(args...)) {}

    private:
        template<typename... Args>
        static std::string createMessage(Args... args) {
            std::ostringstream oss;
            ((oss << args), ...);
            return oss.str();
        }
    };
}

#endif