
#include <logging.h>
#include <eventloop.h>
#include <sockaddr.h>

#include <acceptor.h>
#include <tcpconnection.h>
#include <httpserver.h>

namespace LWF
{

HttpServer::HttpServer(EventLoop *e, SockAddr &addr)
    : event_loop_(e), svc_addr_(addr)
{
    acceptor_ = new Acceptor(e, addr);
}

HttpServer::~HttpServer()
{
}

void HttpServer::start()
{
    acceptor_->start();
}

void HttpServer::run()
{
    while (true) 
    {
        event_loop_->run();
    }
}

TcpConnection* HttpServer::new_connection(EventLoop *e, int fd)
{
    TcpConnection *c = new TcpConnection(e, fd);
    return c;
}

}

