

#include <iostream>

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <sockaddr.h>
#include <eventhandler.h>
#include <selector.h>
#include <eventloop.h>
#include <acceptor.h>
#include <connector.h>
//#include <tcpconnection.h>
//#include <eventhandler.h>

using namespace LWF;

void usage(int argc, char *argv[])
{
    std::cout << "Usage: " << argv[0] << " <Server IP> <Server port>" << std::endl;
}


class MyServiceHandler
{
public:
    MyServiceHandler(EventLoop *e, int fd);
    virtual ~MyServiceHandler();

    virtual int receive(char *buf, int len);
    virtual int transmit(char *buf, int len);

    int handle_input(int fd);
    int handle_output(int fd);

private:
    int         fd_;
    EventLoop   *eventloop_;

    ServiceHandler<MyServiceHandler>    *service_handler_;

};

MyServiceHandler::MyServiceHandler(EventLoop *e, int fd) :
    eventloop_(e), fd_(fd)
{
    service_handler_ = new ServiceHandler<MyServiceHandler>(e);
    service_handler_->set_input_callback(*this, &MyServiceHandler::handle_input);
    service_handler_->set_output_callback(*this, &MyServiceHandler::handle_output);
    eventloop_->register_handler(fd, service_handler_, IOT_READ);
    eventloop_->register_handler(fd, service_handler_, IOT_WRITE);
}

MyServiceHandler::~MyServiceHandler()
{
}

int MyServiceHandler::handle_input(int fd)
{
    char buf[1024];

    LOG_TRACE << "MyServiceHandler::handle_input " << "\r\n";

    receive(buf, 1024);
}

int MyServiceHandler::handle_output(int fd)
{
    char buf[1024];
    std::cin >> buf;
    transmit(buf, strlen(buf));
}

int MyServiceHandler::receive(char *buf, int len)
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

int MyServiceHandler::transmit(char *buf, int len)
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

class TCPClient
{
public:
    TCPClient(EventLoop *e);
    ~TCPClient();

    int connect(SockAddr &addr);

    void run();

private:
    Connector<MyServiceHandler>   *conn_;
    EventLoop   *eventloop_;

};

TCPClient::TCPClient(EventLoop *e) 
{
    eventloop_ = e;
    conn_ = new Connector<MyServiceHandler>(e);
}

TCPClient::~TCPClient()
{
}

int TCPClient::connect(SockAddr &addr)
{
    //Connector<MyServiceHandler>   conn(eventloop_);

    conn_->start(addr);
}

void TCPClient::run()
{
    while (true) 
    {
        eventloop_->run();
    }
}

int main(int argc, char *argv[])
{
    LOG_INFO << "Start testing Connector" << "\r\n";

    if (argc != 3) 
    {
        usage(argc, argv);
        return -1;
    }

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], &srv_addr.sin_addr) < 0) 
    {
        std::cerr << "Invalid IP address: " << argv[1] << std::endl;
        usage(argc, argv);
        return -1;
    }

    SockAddr addr(srv_addr);

    LWF::EventLoop e;

    TCPClient client(&e);
    client.connect(addr);
    client.run();

    return 0;
}

