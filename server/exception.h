#pragma once

#include <stdexcept>
#include <string>

class network_error : public std::runtime_error {
public:
    using _Mybase = std::runtime_error;
    
    explicit network_error(const std::string& message) : _Mybase(message.c_str()) {}
    explicit network_error(const char* message) : _Mybase(message) {}
};

class invalid_socket : public std::runtime_error {
public:
    using _Mybase = std::runtime_error;

    explicit invalid_socket(const std::string& message) : _Mybase(message.c_str()) {}
    explicit invalid_socket(const char* message) : _Mybase(message) {}
};

class unexpected_disconnect : public std::runtime_error {
public:
    using _Mybase = std::runtime_error;

    explicit unexpected_disconnect(const std::string& message) : _Mybase(message.c_str()) {}
    explicit unexpected_disconnect(const char* message) : _Mybase(message) {}
};