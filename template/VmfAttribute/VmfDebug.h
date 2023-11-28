#pragma once
#include <string>
#include <iostream>
#include <sstream>  //std::stringstream
#include <iomanip>  //std::setfill('0')

// using variadic templates

// non template function to call with zero arguments
inline void concatenateStr(std::stringstream &ss) {
}

// base template with 1 argument (which will be called from the variadic one).
template <typename Arg>
void concatenateStr(std::stringstream &ss, Arg arg) {
    //std::cout << __PRETTY_FUNCTION__ << "\n";
    //std::cout << arg << " ";
    ss << arg;
}

// variadic template with one or more arguments.
// ellipsis (...) operator to the left of the parameter name declares a parameter pack,
// allowing you to declare zero or more parameters (of different types).
template <typename First, typename... Args>
void concatenateStr(std::stringstream &ss, First first, Args... args) {
    //ss << __PRETTY_FUNCTION__ << ": ";
    concatenateStr(ss, first);
    concatenateStr(ss, args...);
    //  ellipsis (...) operator to the right of the parameter name will cause
    //  the whole expression that precedes the ellipsis to be repeated for every
    //  subsequent argument unpacked from the argument pack, with the expressions
    //  separated by commas.
}

template <typename... Args>
std::string variadicStr(Args... args) {
    std::stringstream ss("");
    concatenateStr(ss, args...);
    return ss.str();
}

std::string hexDataStr(const void *buffer, size_t N);

#define DEBUG 1
#ifdef DEBUG
#define VmfDebug(...) std::cout << variadicStr(__VA_ARGS__) << '\n'
#define VmfDbg(...) std::cout << variadicStr(__VA_ARGS__)
#else
#define VmfDebug(...)
#define VmfDbg(...)
#endif
#define VmfError(...) std::cout << "[VMF ERR]:\"" << __PRETTY_FUNCTION__ << "\"" <<\
    variadicStr(__VA_ARGS__) << '\n'
#define VmfWarning(...) std::cout <<"[VMF WRN]: " << variadicStr(__VA_ARGS__) << '\n'
