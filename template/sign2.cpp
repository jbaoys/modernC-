#include <type_traits>
#include <iostream>
#include <limits>
#include <climits>

// Primary template for generic case
template <typename T, typename = void>
struct is_enum_signed : std::false_type {};

// Specialization for enumeration types
template <typename T>
struct is_enum_signed<T, typename std::enable_if<std::is_enum<T>::value>::type> {
    static constexpr bool value = std::is_signed<typename std::underlying_type<T>::type>::value;
};


template <typename T, uint8_t N = sizeof(T) *CHAR_BIT>
constexpr T minVal() {
    return (is_enum_signed<T>::value ? T(-(1 << (N-1))) : T(0));
}

template <typename T, uint8_t N>
void yourFunction() {
    if (is_enum_signed<T>::value) {
        // T is a signed type
        // Your code for signed types goes here
        std::cout << "signed\n";
        std::cout << "minval = " << int(minVal<T,N>()) <<'\n';


    } else {
        // T is not a signed type
        // Your code for unsigned types goes here
        std::cout << "unsigned\n";
        std::cout << "minval = " << int(minVal<T,N>()) << '\n';
    }
}
// Example usage
enum class MyEnumSigned : int {};
enum class MyEnumUnsigned : unsigned int {};

int main() {
    yourFunction<MyEnumSigned, 16>(); // T is a signed type
    yourFunction<MyEnumUnsigned,8>(); // T is not a signed type

    return 0;
}

