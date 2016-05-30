


#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <eventhandler.h>
#include <selector.h>
#include <eventloop.h>
//#include <eventhandler.h>


using namespace LWF;

static LWF::EventLoop *g_pEventLoop = NULL;


class Test_Connection : public LWF::EventHandler
{
public:
    Test_Connection(EventLoop *e)
        : EventHandler(e)
    {
    }

    virtual int handle_input(int fd)
    {
        read_callback(fd);
    }

    virtual int get_handler() {
        return fd_;
    }
    virtual void set_handler(int fd) {
        fd_ = fd;
    }
    
private:
    int fd_;

    void read_callback(int fd)
    {
        int rv;
        char buf[1024];
        
        printf("read_callback: fd = %d \r\n", fd);

        memset(buf, 0, sizeof(buf));
        rv = recv(fd, buf, sizeof(buf), 0);
        if (rv < 0)
        {
            std::cerr << "recv error" << std::endl;
            return;
        }
        
        std::cout << "<Receive># " << buf << std::endl;
        
        rv = send(fd, buf, sizeof(buf), 0);
        if (rv < 0)
        {
            std::cerr << "send error" << std::endl;
            return;
        }
        std::cout << "<Send># " << buf << std::endl;
    }
};

class Test_Acceptor : public LWF::EventHandler
{
public:
    Test_Acceptor(EventLoop *e) 
        : EventHandler(e)
    {

    }

    virtual int handle_input(int fd)
    {
        connect_callback(fd);
    }

    virtual int get_handler() {
        return fd_;
    }
    virtual void set_handler(int fd) {
        fd_ = fd;
    }

private:
    int fd_;

    void connect_callback(int fd)
    {
        printf("connect_callback: fd = %d type = %d\r\n", fd);
        
        int cli_sock;
        struct sockaddr_in cli_addr;
        socklen_t          addr_len = sizeof(cli_addr);
        
        cli_sock = accept(fd, (struct sockaddr *)&cli_addr, &addr_len);
        if (cli_sock < 0)
        {
            std::cerr << "accept error" << std::endl;
            return;
        }
        
        std::cout << "New connection from# " << inet_ntoa(cli_addr.sin_addr) << ":" << ntohs(cli_addr.sin_port) << std::endl;
        
        Test_Connection *c = new Test_Connection(eventloop());
        c->set_handler(cli_sock);
        eventloop()->register_handler(c, IOT_READ); 
    }
};

int main()
{
    LOG_INFO << "Start testing EventLoop" << "\r\n";
    
    LWF::EventLoop e;
    g_pEventLoop = &e;

  
    int                 sock;
    int                 svc_port = 8888;
    struct sockaddr_in  svc_addr;
    int                 backlog = 5;
    int rv = 0;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cout << "socket error." << std::endl;
    }
    
    memset(&svc_addr, 0, sizeof(svc_addr));
    svc_addr.sin_family = AF_INET;
    svc_addr.sin_port   = htons(svc_port);
    svc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    rv = bind(sock, (struct sockaddr *)&svc_addr, sizeof(svc_addr));
    if (rv < 0)
    {
        std::cout << "bind error." << std::endl;
    }
    
    rv = listen(sock, backlog);
    if (rv < 0)
    {
        std::cout << "listen error." << std::endl;
    }
    
    Test_Acceptor *a = new Test_Acceptor(&e);
    a->set_handler(sock);
    e.register_handler(a, IOT_READ);
    
    //LWF::TimerNode n(100, test_timer);
    //n.expire();
    
    while (true) {
        e.run();
    }

    return 0;
}


