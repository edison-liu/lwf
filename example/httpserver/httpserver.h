
#ifndef _LWF_HTTPSERVER_H_
#define _LWF_HTTPSERVER_H_

#include <map>

namespace LWF
{

class EventHandler;
class EventLoop;
class SockAddr;
class Acceptor;
class TcpConnection;

class HttpServer
{
public:
    HttpServer(EventLoop *e, SockAddr &addr);
    ~HttpServer();

    void start();
    void run();

    virtual TcpConnection* new_connection(EventLoop *e, int fd);

private:
    EventLoop   *event_loop_;
    SockAddr    svc_addr_;

    Acceptor    *acceptor_;
    std::map<int, TcpConnection *>  connection_;

};

}

#endif // _LWF_HTTPSERVER_H_

