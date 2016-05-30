
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <sockaddr.h>
#include <eventhandler.h>
#include <selector.h>
#include <eventloop.h>
#include <acceptor.h>
#include <tcpconnection.h>
//#include <eventhandler.h>

using namespace LWF;

void usage(int argc, char *argv[])
{
}

class MyTcpConnection : public TcpConnection
{
public:
    MyTcpConnection(EventLoop *el, int fd);
    ~MyTcpConnection();

    virtual void onMessage();
};

class MyAcceptor : public Acceptor
{
public:
    MyAcceptor(EventLoop *e, SockAddr &a);
    ~MyAcceptor();

    virtual TcpConnection* new_connection(EventLoop *e, int fd);
};

MyAcceptor::MyAcceptor(EventLoop *e, SockAddr &a)
    : Acceptor(e, a)
{
}

MyAcceptor::~MyAcceptor()
{
}

TcpConnection* MyAcceptor::new_connection(EventLoop *e, int fd)
{
    printf("MyAcceptor::new_connection: fd = %d type = %d\r\n", fd);
    
    TcpConnection *c = new MyTcpConnection(e, fd);
    return c;
}

MyTcpConnection::MyTcpConnection(EventLoop *el, int fd)
    : TcpConnection(el, fd)
{
}

MyTcpConnection::~MyTcpConnection()
{
}

void MyTcpConnection::onMessage()
{
    int rv;
    char buf[1024];
    
    printf("onMessage: fd = %d \r\n", get_handler());

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


int main(int argc, char *argv[])
{
    LOG_INFO << "Start testing EventLoop" << "\r\n";
    
    if (argc < 2 || argc > 3) 
    {
        usage(argc, argv);
        return -1;
    }

    int port = 0;
    if (argc == 3) {
        port = atoi(argv[2]);
    } else if (argc == 2) {
        port = atoi(argv[1]);
    }

    SockAddr addr(port);

    LWF::EventLoop e;

    Acceptor *a = new Acceptor(&e, addr);
    a->start();
    
    //e.register_handler(a, IOT_READ);
    
    while (true) {
        e.run();
    }

    return 0;
}

