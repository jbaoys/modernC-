#include "VmfDebug.h"
std::string hexDataStr(const void *buffer, size_t N) {
    const uint8_t* ptr = static_cast<const uint8_t*>(buffer);
    std::stringstream ss("");
    bool first = true;
    for (int i=0; i<N; ++i) {
        if (!first) {
            if (i%16 == 0) {
                ss << '\n';
            } else if (i%8 == 0) {
                ss << "  ";
            }
        }
        ss << std::setfill('0') << std::setw(2) << std::right << std::hex;
        ss << uint16_t(ptr[i]) << ' ';
        first = false;
    }
    return ss.str();
}
