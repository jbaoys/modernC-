#ifndef LICD_H
#define LICD_H

#include <stdint.h>

#define PACKED_STRUCT struct __attribute__ ((__packed__))

namespace LICD {
    constexpr size_t COMMAND_INPUT_SIZE = 255;
    PACKED_STRUCT Header {
        uint8_t beginFlag;
        uint8_t address;
        uint8_t control;
        uint8_t sid;
        uint8_t type;
        uint8_t subType;
        uint8_t infoSize;
    };


    PACKED_STRUCT Footer {
        uint16_t fcs;
        uint8_t endFlag;
    };

    PACKED_STRUCT CmdDelayPair {
        uint8_t cmdId;
        uint16_t delay;
    };
    constexpr size_t MAX_CMD_NUM = COMMAND_INPUT_SIZE / sizeof(CmdDelayPair);

    PACKED_STRUCT HdlcMsg {
        Header header;
        uint8_t payload[COMMAND_INPUT_SIZE];
        Footer footer;

        HdlcMsg(uint8_t addr, uint8_t s, uint8_t t, uint8_t st, uint8_t szInfo);
        HdlcMsg();

        void alignFooter();
        void setSid(uint8_t s);
        void calculateFcs();
    };

    PACKED_STRUCT MultiCmdMsg : public HdlcMsg {
        MultiCmdMsg(const uint8_t st, const CmdDelayPair cmds[], const size_t sz);
        void copyCmds(const CmdDelayPair cmds[], const size_t sz);
        CmdDelayPair* getCmdArray();

        void updateAllDelay(uint16_t delay);

        size_t numOfCmd;
    };

} // namespace LICD

#endif
