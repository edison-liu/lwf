
#include <iostream>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <eventloop.h>
#include <tcpconnection.h>

namespace LWF
{

TcpConnection::TcpConnection(EventLoop *el)
    : EventHandler(el)
{
}

TcpConnection::TcpConnection(EventLoop *el, int fd)
    : EventHandler(el), fd_(fd)
{
}

TcpConnection::~TcpConnection()
{

}

int TcpConnection::handle_input(int fd)
{
    onMessage();
}

int TcpConnection::get_handler()
{
    return fd_;
}

void TcpConnection::set_handler(int fd)
{
    fd_ = fd;
}

int TcpConnection::receive(char *buf, int len)
{
    int rv;

    memset(buf, 0, sizeof(buf));
    rv = recv(fd_, buf, len, 0);
    if (rv < 0)
    {
        std::cerr << "recv error" << std::endl;
        return -1;
    }

    std::cout << "<Receive># " << buf << std::endl;
    return rv;
}

int TcpConnection::transmit(char *buf, int len)
{
    int rv;

    rv = send(fd_, buf, sizeof(buf), 0);
    if (rv < 0)
    {
        std::cerr << "send error" << std::endl;
        return -1;
    }
    std::cout << "<Send># " << buf << std::endl;
}

void TcpConnection::onMessage()
{
    int rv;
    char buf[1024];
    
    memset(buf, 0, sizeof(buf));
    rv = receive(buf, sizeof(buf));
    if (rv < 0)
    {
        std::cerr << "recv error" << std::endl;
        return;
    }
    rv = transmit(buf, sizeof(buf));
    if (rv < 0)
    {
        std::cerr << "send error" << std::endl;
        return;
    }
}

}

