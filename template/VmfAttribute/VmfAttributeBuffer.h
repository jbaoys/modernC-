#pragma once
#include <cstdint>  // Fixed width integer types (since C++11), int8_t, int16_t,
                    // ...
#include <cstddef>  // size_t
#include <string.h> // memset
#include <climits>
#include "VmfDebug.h"

class VmfAttributeBuffer {
   public:
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
        VmfDebug("bitInByte_=",bitInByte_);

        return true;
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

    bool setContentSize(size_t size) {
        if (size > size_) return false;
        lastByte_ = size;
        lastBitInByte_ = 0;
        return true;
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

