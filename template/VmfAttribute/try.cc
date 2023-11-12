#include <string>
#include <iostream>
#include <sstream>  //std::stringstream
// non template function to call with zero arguments
void Foo(std::stringstream &ss) {
}

// base template with 1 argument (which will be called from the variadic one).
template <typename Arg>
void Foo(std::stringstream &ss, Arg arg) {
    //std::cout << __PRETTY_FUNCTION__ << "\n";
    //std::cout << arg << " ";
    ss << arg;
}

// variadic template with one or more arguments.
// ellipsis (...) operator to the left of the parameter name declares a parameter pack,
// allowing you to declare zero or more parameters (of different types).
template <typename First, typename... Args>
void Foo(std::stringstream &ss, First first, Args... args) {
    //std::cout << __PRETTY_FUNCTION__ << "\n";
    Foo(ss, first);
    Foo(ss, args...);
    //  ellipsis (...) operator to the right of the parameter name will cause
    //  the whole expression that precedes the ellipsis to be repeated for every
    //  subsequent argument unpacked from the argument pack, with the expressions
    //  separated by commas.
}

template <typename... Args>
std::string LogStr(Args... args) {
    std::stringstream ss("");
    Foo(ss, args...);
    return ss.str();
}

#ifdef DEBUG
#define DEBUG_MSG(...) std::cout << LogStr(__VA_ARGS__) << '\n'
#else
#define DEBUG_MSG(...)
#endif
int main() {
    std::string one = "One";
    const char* two = "Two";
    float three = 3.3333333333;

    DEBUG_MSG(); // non template

    DEBUG_MSG(one); // base template

    DEBUG_MSG(one, " ", two); // variadic argument template

    DEBUG_MSG(one, " ", two,"-", three);  // variadic argument template

    DEBUG_MSG(1, ";",2,";", three,";", 4,";", 5.7,";", 6/2,";", "lalala");  // variadic argument template
    return 0;
}
