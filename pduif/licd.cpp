#include <string.h>
#include "licd.h"

using namespace LICD;

HdlcMsg::HdlcMsg() {
    void* startAddr = static_cast<void*>(this);
    memset(startAddr, 0, sizeof(HdlcMsg));
    header.beginFlag = 0x7E;
    header.control = 0x03;
    footer.endFlag = 0x7E;
}

HdlcMsg::HdlcMsg(uint8_t addr, uint8_t s, uint8_t t, uint8_t st, uint8_t szInfo)
: HdlcMsg() {
    header.address = addr;
    header.sid = s;
    header.type = t;
    header.subType = st;
    header.infoSize = szInfo;
}

void HdlcMsg::alignFooter() {
    if (header.infoSize < COMMAND_INPUT_SIZE) {
        void* newAddr = static_cast<void*> (&payload[header.infoSize]);
        Footer* newFooterPtr = static_cast<Footer*>(newAddr);
        Footer tmpFooter = footer;
        *newFooterPtr = tmpFooter;
    }
}

void HdlcMsg::setSid(uint8_t s) {
    header.sid = s;
}

void HdlcMsg::calculateFcs() {
    if (header.infoSize <= COMMAND_INPUT_SIZE) {
        void* startAddr = static_cast<void*>(this);
        uint16_t seed = *(static_cast<uint16_t*>(startAddr)) + 0xffff;
        footer.fcs = seed;
    }
}

MultiCmdMsg::MultiCmdMsg(const uint8_t st, const CmdDelayPair cmds[], const size_t sz)
: HdlcMsg(0x65, 0x00, 0xC0, st, sz*sizeof(CmdDelayPair)) {
    copyCmds(cmds, sz);
}

void MultiCmdMsg::copyCmds(const CmdDelayPair cmds[], const size_t sz) {
    CmdDelayPair* commands = getCmdArray();
    for (int i=0; i<MAX_CMD_NUM && i<sz; ++i) {
        commands[i] = cmds[i];
    }
    numOfCmd = sz;
}


CmdDelayPair* MultiCmdMsg::getCmdArray() {
    void* rawPayload = static_cast<void*>(payload);
    return static_cast<CmdDelayPair*>(rawPayload);
}

void MultiCmdMsg::updateAllDelay(uint16_t delay) {
    CmdDelayPair* commands = getCmdArray();
    for (int i=0; i<MAX_CMD_NUM && i<numOfCmd; ++i) {
        commands[i].delay = delay;
    }
}

