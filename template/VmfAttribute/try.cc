#include "VmfDebug.h"
int main() {
    std::string one = "One";
    const char* two = "Two";
    float three = 3.3333333333;
    char buf[128] = {
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37
    };

    VmfDebug(); // non template

    VmfDebug(one); // base template

    VmfDebug(one, " ", two); // variadic argument template

    VmfDebug(one, " ", two,"-", three);  // variadic argument template

    VmfDebug(1, ";",2,";", three,";", 4,";", 5.7,";", 6/2,";", "lalala");  // variadic argument template

    std::string hexstr = hexDataStr(two, 3);
    std::cout << hexstr << '\n';
    std::cout << hexDataStr(one.c_str(),3) << '\n';
    std::cout << hexDataStr(buf, 37) << '\n';
    return 0;
}
