#include <cstdint>
#include <type_traits>
#include <array>
#include <string>
#include <iostream>
#include <sstream>  //std::stringstream
template<typename T>
T set_top_bit(T value, bool state) {
    //constexpr T mask = T(1) << (sizeof(T)*8-1);
    constexpr T mask = ~(T(1) << (sizeof(T)*8-1));
    return mask;
    /*
    value &= mask;
    if (state) value |= ~mask;
    return value;
    */
}

int main()
{
    int16_t vv= 0x12;
    std::cout << std::hex << int(set_top_bit<int32_t>(vv, false)) << '\n';
    return 0;

}
