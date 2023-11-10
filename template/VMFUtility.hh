#ifndef _VMFATTRIBUTE_
#define _VMFATTRIBUTE_
#include <type_traits>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>



class VMFAttribute
{
public:
    VMFAttribute(size_t numOfBits)
    : numOfBits_(numOfBits) {
    }
    size_t getNumOfBits() { return numOfBits_; }
    virtual void encode(char* buf) = 0;
    virtual void decode(char* buf) = 0;
private:
    size_t numOfBits_;
};

class PlainAttribute : public VMFAttribute
{
public:
    PlainAttribute(size_t num) : VMFAttribute(num) {}
};

class VMFGroup : public VMFAttribute
{
public:
    VMFGroup(size_t num) : VMFAttribute(num) {}
};

template<typename T, size_t bits, T min, T max>
class intAttribute : public PlainAttribute
{
public:
    intAttribute(T v)
    : value(v), PlainAttribute(bits) {}
    void encode(char* buf) final
    {
    }
    void decode(char* buf) final
    {
    }
private:
    T value;
};

template <class T>
class indicatorAttribute : public VMFAttribute
{
public:
    indicatorAttribute(T pa)
    : present(false)
    , value(pa)
    , VMFAttribute(1) {}
    void encode(char* buf)
    {
        // ecode present
        if (present)
            value.encode(buf);
    }

    void decode(char* buf)
    {
        // decode present
        if (present)
            value.decode(buf);
    }

private:
    bool present;
    T value;
};

template <class TVMFPlainAttribute>
class VMFOptionalAttribute : public indicatorAttribute<TVMFPlainAttribute>
{
    static_assert( std::is_base_of<PlainAttribute, TVMFPlainAttribute>::value,
            "TVMFPlainAttribute must be a PlainAttribute type" );
public:
    VMFOptionalAttribute(TVMFPlainAttribute pa)
    : indicatorAttribute<TVMFPlainAttribute>::indicatorAttribute(pa) {
    }
};

template <class TVMFGroup>
class VMFOptionalGroup : public indicatorAttribute<TVMFGroup>
{
    static_assert( std::is_base_of<VMFGroup, TVMFGroup>::value,
            "TVMFGroup must be a VMFGroup type" );
public:
    VMFOptionalGroup(TVMFGroup pa)
    : indicatorAttribute<TVMFGroup>::indicatorAttribute(pa) {
    }
};

#endif
