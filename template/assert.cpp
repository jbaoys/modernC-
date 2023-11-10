#include <type_traits>
static_assert(03301 == 1729); // since C++17 the message string is optional
template<class T>
void swap(T& a, T& b) noexcept
{
    static_assert(std::is_copy_constructible_v<T>,
                  "Swap requires copying");
    static_assert(std::is_nothrow_copy_constructible_v<T> &&
                  std::is_nothrow_copy_assignable_v<T>,
                  "Swap requires nothrow copy/assign");
    auto c = b;
    b = a;
    a = c;
}
template<class T>
struct data_structure
{
    static_assert(std::is_default_constructible_v<T>,
                  "Data structure requires default-constructible elements");
};
template<class>
constexpr bool dependent_false = false; // workaround before CWG2518/P2593R1
template<class T>
struct bad_type
{
    static_assert(dependent_false<T>, "error on instantiation, workaround");
    static_assert(false, "error on instantiation"); // OK because of CWG2518/P2593R1
};
struct no_copy
{
    no_copy(const no_copy&) = delete;
    no_copy() = default;
};
struct no_default
{
    no_default() = delete;
};
#if __cpp_static_assert >= 202306L
#include <format>
// Not real C++ yet (std::format should be constexpr to work):
static_assert(sizeof(int) == 4, std::format("Expected 4, got {}", sizeof(int)));
#endif
int main()
{
    int a, b;
    swap(a, b);
    no_copy nc_a, nc_b;
    swap(nc_a, nc_b); // 1
    [[maybe_unused]] data_structure<int> ds_ok;
    [[maybe_unused]] data_structure<no_default> ds_error; // 2
}
