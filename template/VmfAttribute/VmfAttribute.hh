#pragma once
#include <cstdint>
#include <type_traits>
#include <array>
#include <string>
#include <iostream>
#include <sstream>  //std::stringstream

constexpr char DELIMITER='`';

class VmfAttribute {
   public:
    VmfAttribute(uint16_t bits) : numberOfBits_(bits) {
    }
    virtual ~VmfAttribute() = default;
    virtual bool encode(uint8_t* toBuffer) = 0;
    virtual bool decode(uint8_t* fromBuffer) = 0;

    uint16_t getNumberOfBits() {
        return numberOfBits_;
    }
   private:
    uint16_t numberOfBits_;
};

class VmfPlainAttribute : public VmfAttribute {
   public:
    VmfPlainAttribute(int16_t bits) : VmfAttribute(bits) {}
    virtual ~VmfPlainAttribute() = default;
};

class PlainGroup : public VmfAttribute {
   public:
    // TODO: find out the sum of all memebers bits in compiling time
    PlainGroup(int16_t bits) : VmfAttribute(bits) {}
    virtual ~PlainGroup() = default;
};

template<typename T, uint8_t numbits, T minValue, T maxValue, uint16_t N = 1>
class IntAttribute : public VmfPlainAttribute {
    static_assert(numbits <= sizeof(T)*8, "numbits it too big!!");
    constexpr static T Tmin = (numbits < sizeof(T)*8) ? -(T(1)<<(numbits-1)) : T(1) << (numbits-1);
    constexpr static T Tmax = (numbits < sizeof(T)*8) ? ((T(1)<<(numbits-1)) - 1) : T(~(T(1) << (numbits-1)));
    //constexpr static T Tmin = T(1) << (numbits-1);
    //constexpr static T Tmax = ~(T(1) << (numbits-1));
    static_assert(minValue >= Tmin, "minValue it too small!!");
    static_assert(maxValue <= Tmax, "maxValue it too big!!");
   public:
    IntAttribute(T value)
    : VmfPlainAttribute(numbits*N) , values_({value}) {
        if (!isValid(value)) std::cout << "Invalid value " << int(value) << "!\n";
    }
    IntAttribute(T values[N])
    : VmfPlainAttribute(numbits*N) , values_(values) {
        for (int i=0; i<N; i++) {
        if (!isValid(values[i])) std::cout << "Invalid value " << int(values[i]) << "!\n";
        }
    }
    IntAttribute(std::array<T,N> values)
    : VmfPlainAttribute(numbits*N) , values_(values) {
        for (int i=0; i<N; i++) {
        if (!isValid(values[i])) std::cout << "Invalid value " << int(values[i]) << "!\n";
        }
    }
    IntAttribute() : VmfPlainAttribute(numbits) {
    }
    virtual ~IntAttribute() = default;
    bool encode(uint8_t *toBuffer) {
        std::stringstream ss("");
        for (int i=0; i<N; i++) {
            ss << (int)values_[i] << DELIMITER;
        }
        memcpy(toBuffer, ss.str().c_str(), sizeof(ss.str()));
        std::cout << ss.str() << '\n';
        return true;
    }
    bool decode(uint8_t *toBuffer) {
        return false;
    }
    T getValue() {
        return values_[0];
    }
    // get values_ to std::array
    /*
    std::array<T, N> getValues() {
        return values_;
    }
    */
    // get values to plain array
    void getValues(T (&values)[N]) {
        for (int i=0; i<N; i++) values[i] = values_[i];
    }
    bool setValue(T value) {
        if (!isValid(value)) return false;
        values_[0] = value;
        return true;
    }
    // set values from a plain array reference
    bool setValues(T (&values)[N]) {
        for ( int i=0; i<N; i++ ) {
            if (!isValid(values[i])) return false;
            else values_[i] = values[i];
        }
        return true;
    }
    // set values from std::array
    bool setValues(std::array<T, N> &values) {
        for ( int i=0; i<N; i++ ) {
            if (!isValid(values[i])) return false;
        }
        values_ = values;
        return true;
    }
   private:
    bool isValid(T &v) {
        return (v >= minValue && v <= maxValue);
    }
    std::array<T, N> values_;
};
