#include <cmath>
#include <iostream>
#include <string>

#include "ConstexprConfig.h"
constexpr double const_sqrt(double x) {
    return sqrt(x);
}
/*
 * Now you can use factorial( 2 ) and when the compiler sees it, it can optimize away the call and make the calculation entirely at compile time. In this way, by allowing more sophisticated calculations, constexpr behaves differently than a mere inline function. You can't inline a recursive function! In fact, any time the function argument is itself a constexpr, it can be computed at compile time.
 */
constexpr int factorial(int n) {
    return n > 0 ? n * factorial( n - 1 ) : 1;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << argv[0]
            << "Version " << Constexpr_VERSION_MAJOR << "."
            << Constexpr_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }
    const double inputValue = std::stod(argv[1]);
    constexpr double inputValue2 = 29424091;
    //This will cause error:
    // error: the value of ‘inputValue’ is not usable in a constant expression
    //constexpr double outputValue = const_sqrt(inputValue);
    constexpr double outputValue = const_sqrt(inputValue2);
    std::cout << "The root of " << inputValue2 << " is " << outputValue << '\n';
    std::cout << "The root of " << argv[1] << " is " << sqrt(inputValue) << '\n';

    constexpr int v = 7;
    constexpr int f = factorial(v);
    std::cout << v << "! = " << f << '\n';

    return 0;
}
