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
   public:
    IntAttribute(T value)
    : VmfPlainAttribute(numbits) , values_({value}) {
    }
    IntAttribute(std::array<T,N> values)
    : VmfPlainAttribute(numbits) , values_(values) {
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
    std::array<T, N> getValues() {
        return values_;
    }
    bool setValue(T value) {
        if (value < minValue || value > maxValue) return false;
        values_[0] = value;
        return true;
    }
    bool setValues(std::array<T, N> &values) {
        for ( int i=0; i<N; i++ ) {
            if (values[i] < minValue || values[i] > maxValue) return false;
        }
        values_ = values;
        return true;
    }
   private:
    std::array<T, N> values_;
};
