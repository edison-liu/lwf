
#include <iostream>

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <eventhandler.h>
#include <eventloop.h>
#include <connector.h>

#include <of_connection.h>

namespace LWF
{
namespace OPENFLOW
{

OFConnection::OFConnection(EventLoop *e, int fd) :
    eventloop_(e), fd_(fd)
{
    conn_ = new ServiceHandler<OFConnection>(e);
    conn_->set_input_callback(*this, &OFConnection::handle_input);
    conn_->set_output_callback(*this, &OFConnection::handle_output);
    eventloop_->register_handler(fd, conn_, IOT_READ);
    eventloop_->register_handler(fd, conn_, IOT_WRITE);
}

OFConnection::~OFConnection()
{
}

int OFConnection::handle_input(int fd)
{
    char buf[1024];

    LOG_TRACE << "OFConnection::handle_input " << "\r\n";

    receive(buf, 1024);
}

int OFConnection::handle_output(int fd)
{
    char buf[1024];
    std::cin >> buf;
    transmit(buf, strlen(buf));
}

int OFConnection::receive(char *buf, int len)
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

int OFConnection::transmit(char *buf, int len)
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

}
}

