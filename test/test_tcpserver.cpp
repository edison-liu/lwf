

#include <iostream>

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
#include <tcpconnection.h>
//#include <eventhandler.h>

using namespace LWF;

void usage(int argc, char *argv[])
{
}

class TcpServer
{
public:
    TcpServer(EventLoop *e, SockAddr &addr);
    ~TcpServer();

    void start();
    void run();

    TcpConnection* on_new_connection(int fd, SockAddr &addr);

private:
    EventLoop   *event_loop_;
    SockAddr    srv_addr_;

    Acceptor    *acceptor_;

};

TcpServer::TcpServer(EventLoop *e, SockAddr &addr)
    : event_loop_(e), srv_addr_(addr)
{
    acceptor_ = new Acceptor(e, addr);
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    acceptor_->start();
}

void TcpServer::run()
{
    while (true) 
    {
        event_loop_->run();
    }
}

TcpConnection* TcpServer::on_new_connection(int fd, SockAddr &addr)
{
    TcpConnection *c = new TcpConnection(event_loop_, fd);
    return c;
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

    TcpServer server(&e, addr);
    server.start();
    server.run();

    return 0;
}

