
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <eventloop.h>
#include <sockaddr.h>
#include <acceptor.h>
#include <tcpconnection.h>

namespace LWF
{

Acceptor::Acceptor(EventLoop *e, SockAddr &a)
    : EventHandler(e), event_loop_(e), addr_(a)
{
}

Acceptor::~Acceptor()
{
}

int Acceptor::listen_i()
{
    int                 sock;
    int                 backlog = 5;
    int rv = 0;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cout << "socket error." << std::endl;
    }
    
    LOG_TRACE << "Bind to# " << inet_ntoa((addr_.addr())->sin_addr) << ":" 
              << ntohs((addr_.addr())->sin_port) << "\r\n";
    rv = bind(sock, (struct sockaddr *)addr_.addr(), addr_.length()); 
    if (rv < 0)
    {
        std::cout << "bind error." << std::endl;
    }
    
    rv = listen(sock, backlog);
    if (rv < 0)
    {
        std::cout << "listen error." << std::endl;
    }

    fd_ = sock;

    event_loop_->register_handler(this, IOT_READ);

}

int Acceptor::start()
{
    listen_i();
}

int Acceptor::start(const SockAddr &a)
{
    addr_ = a;
    
    listen_i();
}

int Acceptor::close()
{
}

int Acceptor::handle_input(int fd)
{
    onConnection();
}

int Acceptor::get_handler()
{
    return fd_;
}

void Acceptor::set_handler(int fd)
{
    fd_ = fd;
}

void Acceptor::onConnection()
{
    int cli_sock;
    struct sockaddr_in cli_addr;
    socklen_t          addr_len = sizeof(cli_addr);
    
    cli_sock = accept(fd_, (struct sockaddr *)&cli_addr, &addr_len);
    if (cli_sock < 0)
    {
        std::cerr << "accept error" << std::endl;
        return;
    }
    
    std::cout << "New connection from# " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << std::endl;
    
    TcpConnection *conn = new_connection(event_loop_, cli_sock);
    event_loop_->register_handler(conn, IOT_READ); 
}

TcpConnection* Acceptor::new_connection(EventLoop *e, int fd)
{
    TcpConnection *c = new TcpConnection(e, fd);
    return c;
}

}

