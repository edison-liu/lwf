
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <eventloop.h>

static LWF::EventLoop *g_pEventLoop = NULL;

void test_timer()
{
    printf("Test timer\r\n");
}

void test_timer1()
{
    printf("Test timer 1\r\n");
}

void read_callback(int fd, LWF::IoEventType type)
{
    int rv;
    char buf[1024];
    
    printf("read_callback: fd = %d type = %d\r\n", fd, type);

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

void connect_callback(int fd, LWF::IoEventType type)
{
    printf("connect_callback: fd = %d type = %d\r\n", fd, type);
    
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
    
    g_pEventLoop->add_ioevent(cli_sock, LWF::IOT_READ, read_callback);
}


int main()
{
    LOG_INFO << "Start testing EventLoop" << "\r\n";
    
    LWF::EventLoop e;
    g_pEventLoop = &e;

    //e.new_periodic_ms(100, test_timer);
    //e.new_periodic_ms(100, test_timer1);
    
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
    
    e.add_ioevent(sock, LWF::IOT_READ, connect_callback);
    
    //LWF::TimerNode n(100, test_timer);
    //n.expire();
    
    while (true) {
        e.run();
    }

    return 0;
}

