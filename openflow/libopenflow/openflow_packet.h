#ifndef _LWF_OPENFLOW_PACKET_H_
#define _LWF_OPENFLOW_PACKET_H_

#include <stdint.h>

namespace LWF
{

namespace OPENFLOW
{


struct ofp_header
{
    uint8_t     version;
    uint8_t     type;
    uint16_t    length;
    uint32_t    xid;
};



}

}

#endif // _LWF_OPENFLOW_PACKET_H_

