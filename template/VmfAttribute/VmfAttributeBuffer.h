#pragma once
#include <cstdint>  // Fixed width integer types (since C++11), int8_t, int16_t,
                    // ...
#include <cstddef>  // size_t
#include <climits>
#include <string.h> // memset
#include <climits>
#include "VmfDebug.h"

class VmfAttributeBuffer {
   public:
    static const size_t SZU64 = sizeof(uint64_t);
    union byte64 {
        byte64(const uint64_t& v) : U64(v) {}
        byte64() : U64(0) {}
        uint64_t U64;
        uint8_t U8[sizeof(U64)];
    };
    VmfAttributeBuffer(uint8_t *buffer, size_t size)
    : buffer_(buffer)
    , size_(size)
    , currentByte_(0)
    , bitInByte_(0)
    , lastByte_(0)
    , lastBitInByte_(0) {
    }

    virtual ~VmfAttributeBuffer() = default;

    size_t getSize() {
        return size_;
    }

    size_t getContentSize()
    {
        return lastBitInByte_ ? lastByte_ + 1 : lastByte_;
    }

    size_t getCurrentBitPos()
    {
        return currentByte_ * CHAR_BIT + bitInByte_;
    }

    size_t getContentBitSize()
    {
        return lastByte_ * CHAR_BIT + lastBitInByte_;
    }

    void fillVoid() {
        memset(buffer_, 0, size_);
    }
    void resetIterator() {
        currentByte_ = 0;
        bitInByte_ = 0;
    }
    void purgeBuffer() {
        fillVoid();
        resetIterator();
        lastByte_ = 0;
        lastBitInByte_ = 0;
    }

    bool appendBit(uint8_t bit) {
        if (currentByte_ >= size_) {
            VmfError("Buffer is full with size of ", size_);
            return false;
        }
        uint8_t mask = 1 << bitInByte_;
        if (bit) {
            buffer_[currentByte_] |= mask;
        } else {
            buffer_[currentByte_] &= (~mask);
        }
        increaseBit();
        lastByte_ = currentByte_;
        lastBitInByte_ = bitInByte_;
        //VmfDebug("bitInByte_=",bitInByte_);

        return true;
    }

    bool appendBits(const uint64_t &bits, size_t numBits) {
        bool retVal = false;
        int endingPos = numBits + bitInByte_;
        uint8_t newBitInByte = uint8_t(endingPos & 0x7);
        //check numBits
        size_t requiredBytes = static_cast<size_t>((endingPos >> 3) +
                               (newBitInByte != 0));
        if ((requiredBytes + currentByte_) <= size_) {
            byte64 dataBits = bits;
            uint8_t firstByteMask = uint8_t((1u << bitInByte_) - 1);
            uint8_t lastByteMask = uint8_t((1u << newBitInByte) - 1);
            uint8_t overflowByte = ( dataBits.U8[SZU64-1] >>
                                     (CHAR_BIT - bitInByte_) );

            dataBits.U64 <<= bitInByte_;
            dataBits.U8[0] |= (buffer_[currentByte_] & firstByteMask);
            int index =0;
            for (; index < requiredBytes && index < SZU64; ++index) {
                buffer_[currentByte_ + index] = dataBits.U8[index];
            }
            if (index < requiredBytes) {
                buffer_[currentByte_ + index] = overflowByte;
                ++index;
            }
            --index;
            currentByte_ += index;
            bitInByte_ = newBitInByte;
            if (lastByteMask == 0) {
                currentByte_++;
            } else {
                buffer_[currentByte_] &= lastByteMask;
            }

            lastByte_ = currentByte_;
            lastBitInByte_ = bitInByte_;

            retVal = true;
        }

        return retVal;
    }


    bool getBit(uint8_t &bit) {
        size_t conSz = getContentSize();
        if (currentByte_ >= conSz) {
            VmfError("Reach the end of buffer with content size of ", conSz);
            return false;
        }
        if ((currentByte_ == lastByte_) && (bitInByte_ >= lastBitInByte_)) {
            VmfError("Reach the end of buffer with content size of ", conSz,
                    ", bit offset at ", lastBitInByte_);
            return false;
        }
        uint8_t mask = 1 << bitInByte_;
        if ( mask & buffer_[currentByte_] ) {
            bit = 1;
        } else {
            bit = 0;
        }

        increaseBit();

        return true;
    }

    bool getBits(uint64_t &bits, size_t numBits) {
        bool retVal = false;
        int endingPos = numBits + bitInByte_;
        size_t newBitInByte = static_cast<size_t>(endingPos & 0x7);
        //check numBits
        size_t requiredBytes = static_cast<size_t>((endingPos >> 3) +
                               (newBitInByte != 0));
        size_t newConBitSz = getCurrentBitPos() + numBits;
        //VmfDebug("newConBitSz=",newConBitSz);
        //VmfDebug("currentConBitSize=",getContentBitSize());

        if ((numBits <= SZU64*CHAR_BIT) && (newConBitSz <= getContentBitSize())) {
            byte64 dataBits;
            uint8_t firstByteMask = uint8_t((1u << bitInByte_) - 1);
            uint8_t lastByteMask = uint8_t((1u << newBitInByte) - 1);
            uint8_t higherByte = 0;

            //dataBits.U8[0] |= (buffer_[currentByte_] & firstByteMask);
            int index =0;
            bool useHigherByte = false;
            //VmfDebug("requiredBytes=",requiredBytes);
            //VmfDebug("bitInByte_=",bitInByte_);
            for (; index < requiredBytes && index < SZU64; ++index) {
                dataBits.U8[index] = buffer_[currentByte_ + index];
            }
            if (index < requiredBytes) {
                higherByte = buffer_[currentByte_ + index];
                if (lastByteMask != 0) {
                    higherByte &= lastByteMask;
                }
                useHigherByte = true;
            } else {
                --index;
                if (lastByteMask != 0) {
                    dataBits.U8[index] &= lastByteMask;
                }
            }
            dataBits.U64 >>= bitInByte_;
            if (useHigherByte) {
                dataBits.U8[SZU64-1] |= (higherByte << (CHAR_BIT - bitInByte_));
            }
            currentByte_ += index;
            bitInByte_ = newBitInByte;
            if (lastByteMask == 0) {
                currentByte_++;
            }
            //VmfDebug("dataBits.U64=", std::hex, dataBits.U64);
            bits = dataBits.U64;
            retVal = true;
        } else {
            VmfError("Reach the end of buffer with content bit size of ",
                    getContentBitSize(), "\n new size is ", newConBitSz,
                    ", numBits=",numBits,", SZU64=",SZU64);
        }

        return retVal;
    }

    bool setContentSize(size_t size) {
        if (size > size_) return false;
        lastByte_ = size;
        lastBitInByte_ = 0;
        return true;
    }

    bool setContentBitSize(size_t numBits) {
        //check numBits
        bool retVal = false;
        size_t extraBytes = numBits >> 3;
        size_t extraBits = numBits & 0x7;
        size_t requiredBytes = extraBytes + (extraBits != 0);
        //VmfDebug("requiredBytes=", requiredBytes);
        if (requiredBytes <= size_) {
            lastByte_ = extraBytes;
            lastBitInByte_ = extraBits;
            retVal = true;
        }
        return retVal;
    }

   private:
    void increaseBit() {
        bitInByte_++;
        if (bitInByte_ >= CHAR_BIT) {
            bitInByte_ = 0;
            currentByte_++;
        }
    }
    uint8_t *buffer_;
    size_t size_;
    size_t currentByte_;
    size_t bitInByte_;
    size_t lastByte_;
    size_t lastBitInByte_;
};

