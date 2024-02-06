#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>
#include "licd.h"

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

int main(int argc, char* argv[])
{
    LICD::HdlcMsg myMsg;

    std::cout << "Size of myMsg = " << sizeof(myMsg) << '\n';
    std::cout << "myMsg.beginFlag = " << std::hex << int(myMsg.header.beginFlag) << '\n';
    std::cout << "myMsg.address = " << std::hex << int(myMsg.header.address) << '\n';
    std::cout << "myMsg.control = " << std::hex << int(myMsg.header.control) << '\n';
    std::cout << "myMsg.endFlag = " << std::hex << int(myMsg.footer.endFlag) << '\n';
    myMsg.footer.fcs = 0x1234;
    myMsg.header.infoSize = 254;
    myMsg.alignFooter();
    std::cout << "myMsg raw data =\n" << hexDataStr(static_cast<void*>(&myMsg), sizeof(myMsg)) << '\n';

    constexpr LICD::CmdDelayPair myCmds[2] = { {0xa0, 0x1234}, {0xb0, 0x5678} };

    const LICD::MultiCmdMsg C_PCS(0x2c, myCmds, 2);
    LICD::MultiCmdMsg pcs(C_PCS);
    pcs.setSid(0x5a);
    pcs.calculateFcs();
    pcs.alignFooter();
    std::cout << "pcs raw data =\n" << hexDataStr(static_cast<void*>(&pcs), sizeof(pcs)) << '\n';
    pcs.updateAllDelay(0x010f);
    std::cout << "pcs raw data =\n" << hexDataStr(static_cast<void*>(&pcs), sizeof(pcs)) << '\n';

    return 0;
}
