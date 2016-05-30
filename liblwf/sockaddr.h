
#ifndef _LWF_SOCKADDR_H_
#define _LWF_SOCKADDR_H_

#include <netinet/in.h>

namespace LWF
{

class SockAddr
{
public:
    SockAddr(uint16_t port=0, bool loopbackonly = false);
    SockAddr(const char* str_addr, uint16_t port);
    SockAddr(const struct sockaddr_in &sin);
    SockAddr(const SockAddr &a);
    SockAddr();
    virtual ~SockAddr();

    virtual SockAddr& operator=(const SockAddr &a);

    struct sockaddr_in* addr();
    int length();

private:
    union
    {
        struct sockaddr_in  addr_;
        struct sockaddr_in6 addr6_;
    };
};

}

#endif // _LWF_SOCKADDR_H_

