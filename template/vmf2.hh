// THIS IS AUTO GENERATED CODE, PLEASE DO NOT EDIT!!
// GENERATED ON 2023/10/15, 14:48:20

#include "VMFUtility.hh"
#ifndef _E_IFTU_
#define _E_IFTU_


struct E_IFTU : public VMFAttribute {
    E_IFTU(AUR_ID f0)
    : aur_id(f0)
    {
    }


    E_IFTU()
    : aur_id()
    {
    }


    bool encode(VMFAttibuteBuffer& tobuf) {
        const char* memberField = nullptr;
        do {
            if (!aur_id.encode(tobuf)) {
                memberField = "aur_id";
                break;
            }
            return true;
        } while (0);
        ERROR("E_IFTU, field<", memberField, ">: failed to encode!!!");
        return false;
    }


    bool decode(VMFAttibuteBuffer& frombuf) {
        const char* memberField = nullptr;
        do {
            if (!aur_id.decode(frombuf)) {
                memberField = "aur_id";
                break;
            }
            return true;
        } while (0);
        ERROR("E_IFTU, field<", memberField, ">: failed to decode!!!");
        return false;
    }


    // Group field members:
    AUR_ID  aur_id;
};

/* Init pattern

         // PRESENT
} // </E_IFTU>

*/

#endif // _E_IFTU_
