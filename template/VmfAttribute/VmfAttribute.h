#pragma once
#include <cstdint>
#include <type_traits>
#include <array>
#include <string>
#include <iostream>
#include <sstream>  //std::stringstream
#include "VmfAttributeBuffer.h"

constexpr char DELIMITER='`';

class VmfAttribute {
   public:
    VmfAttribute(uint16_t bits) : numberOfBits_(bits) {
    }
    virtual ~VmfAttribute() = default;
    virtual bool encode(VmfAttributeBuffer &toBuffer) = 0;
    virtual bool decode(VmfAttributeBuffer &fromBuffer) = 0;

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

template<class T>
using unsignT = typename std::make_unsigned<T>::type;

// Calculate minium valid value based on bit width
template<typename T, uint8_t N>
constexpr T minVal() {
    //return (N < sizeof(T)*8) ? -(T(1)<<(N-1)) : T(1) << (N-1);
    return (T(-1)<T(0)) ? ((N < sizeof(T)*8) ? static_cast<T>(-(unsignT<T>(1)<<(N-1))) :
            T(unsignT<T>(1) << (N-1))) : T(0);
};
// Calculate maximum valid value based on bit width
template<typename T, uint8_t N>
constexpr T maxVal() {
    //return (N < sizeof(T)*8) ? ((T(1)<<(N-1)) - 1) : T(~(T(1) << (N-1)));
    return (T(-1)<T(0)) ? T(((unsignT<T>(1)<<(N-1)) - 1)) :
           T((unsignT<T>(1)<<N)-1);
};

/**
 * @Tparam T        Data type used to hold attributes
 * @Tparam numbits  Bit width, must be within the maximum width of T
 * @Tparam minValue Range value
 * @Tparam maxValue Range value
 * @Tparam N        Multiplicity
 */
template<typename T, uint8_t numbits, T minValue, T maxValue, uint16_t N = 1>
class IntAttribute : public VmfPlainAttribute {
    static_assert(numbits <= sizeof(T)*8, "numbits it too big!!");
    constexpr static T Tmin = minVal<T, numbits>();
    constexpr static T Tmax = maxVal<T, numbits>();
    static_assert(minValue >= Tmin, "minValue it too small!!");
    static_assert(maxValue <= Tmax, "maxValue it too big!!");

   public:
    IntAttribute(T value)
    : VmfPlainAttribute(numbits*N) , values_({value}) {
        if (!isValid(value)) VmfError("Invalid value ", int(value), "!");
    }

    IntAttribute(T values[N])
    : VmfPlainAttribute(numbits*N) , values_(values) {
        for (int i=0; i<N; i++) {
        if (!isValid(values[i])) VmfError("Invalid value ", int(values[i]), "!");
        }
    }

    IntAttribute(std::array<T,N> values)
    : VmfPlainAttribute(numbits*N) , values_(values) {
        for (int i=0; i<N; i++) {
        if (!isValid(values[i])) VmfError("Invalid value ", int(values[i]), "!");
        }
    }

    IntAttribute() : VmfPlainAttribute(numbits*N) {
        for (int i=0; i<N; i++) {
            values_[i] = 0;
        }
    }

    virtual ~IntAttribute() = default;

    bool encode(VmfAttributeBuffer &toBuffer) {
        for (int i=0; i<N; i++) {
            if (!isValid(values_[i])) {
                VmfError("Invalid IntAttribute values_[", i, "]= ",
                        int(values_[i]), "! Should be within range of (",
                        minValue, ", ", maxValue, "),");
                return false;
            }

            VmfDbg("<", (int)values_[i], ">");
            uint8_t bit;
            uint8_t mask = 1;
            for (uint8_t j=0; j<numbits; j++) {
                bit = (values_[i] & mask) ? 1 : 0;
                if (!toBuffer.appendBit(bit)) {
                    VmfError("Fail to append a bit at bit ",  int(j),
                            " of values_[", i, "]");
                    return false;
                }
                mask <<= 1;
            }
        }
        return true;
    }

    bool decode(VmfAttributeBuffer &fromBuffer) {
        for (int i=0; i<N; i++) {
            uint8_t bit;
            uint8_t mask = 1;
            for (uint8_t j=0; j<numbits; j++) {
                if (!fromBuffer.getBit(bit)) {
                    VmfError("Fail to get a bit at bit ",  int(j),
                            " of values_[", i, "]");
                    return false;
                }
                if (bit) {
                    values_[i] |= mask;
                } else {
                    values_[i] &= (~mask);
                }
                mask <<= 1;
            }
            // set sign
            if (bit) {
                values_[i] |= Tmin;
            } else {
                values_[i] &= Tmax;
            }

            if (!isValid(values_[i])) {
                VmfError("Invalid decoded IntAttribute values_[", i, "]= ",
                        int(values_[i]), "! Should be within range of (",
                        minValue, ", ", maxValue, "),");
                return false;
            }

            VmfDbg("<", (int)values_[i], ">");
        }
        return true;
    }

    // return a single value, only for non-array.
    T getValue() {
        return values_[0];
    }

    // return values_ to std::array, benefit unit tests
    std::array<T, N> getValues() {
        return values_;
    }

    // get values to plain array
    void getValues(T (&values)[N]) {
        for (int i=0; i<N; i++) values[i] = values_[i];
    }

    // set single value
    bool setValue(T value) {
        if (!isValid(value)) return false;
        values_[0] = value;
        return true;
    }

    // set values from a plain array reference
    bool setValues(T (&values)[N]) {
    //bool setValues(T values[N]) {
        VmfDbg("call setValues\n");
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
