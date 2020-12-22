/**
 * Created by Federico Manuel Gomez Peter 
 * on 22/12/20.
 */

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__


#include <exception>
#include <string>

class Exception : public std::exception {
private:
    std::string msg_error;

public:
    explicit Exception(const char* fmt, ...) noexcept;
    Exception() = delete;
    virtual const char* what() const noexcept;
    virtual ~Exception() noexcept = default;
};


#endif //__EXCEPTION_H__
