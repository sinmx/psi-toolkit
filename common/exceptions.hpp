#ifndef EXCEPTIONS_HDR
#define EXCEPTIONS_HDR


#include <stdexcept>
#include <string>


class Exception : public std::runtime_error {
public:
    Exception(std::string message) : std::runtime_error(message), message_(message) { }
    ~Exception() throw() { }
    std::string getMessage() { return message_; }
protected:
    std::string message_;
};


class NoEdgeException : public Exception {
public:
    NoEdgeException(std::string message) : Exception(message) { }
};


class FileFormatException : public Exception {
public:
    FileFormatException(std::string message) : Exception(message) { }
};


#endif