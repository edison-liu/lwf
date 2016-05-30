
#include <arpa/inet.h>

#include <logging.h>
#include <sockaddr.h>

namespace LWF
{

SockAddr::SockAddr(uint16_t port, bool loopbackonly)
{
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port   = htons(port);
    if (loopbackonly == false) 
    {
        addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else 
    {
        addr_.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
}

SockAddr::SockAddr(const char* str_addr, uint16_t port)
{
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port   = htons(port);
    if (inet_aton(str_addr, &addr_.sin_addr)) 
    {
        LOG_ERROR << "inet_aton error" << "\r\n";
    }
}

SockAddr::SockAddr(const struct sockaddr_in &sin)
{
    bzero(&addr_, sizeof(addr_));
    memcpy(&addr_, &sin, sizeof(addr_));
}

SockAddr::SockAddr(const SockAddr &a)
{
    bzero(&addr_, sizeof(addr_));
    memcpy(&addr_, &a.addr_, sizeof(addr_));
}

SockAddr::SockAddr()
{
}

SockAddr::~SockAddr()
{
}

SockAddr& SockAddr::operator=(const SockAddr &a)
{
    bzero(&addr_, sizeof(addr_));
    memcpy(&addr_, &a, sizeof(addr_));
    return *this;
}

struct sockaddr_in* SockAddr::addr()
{
    return &addr_;
}

int SockAddr::length()
{
    return sizeof(addr_);
}

}



