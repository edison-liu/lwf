
#include <iostream>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <logging.h>
#include <sockaddr.h>
#include <eventloop.h>
#include <httpserver.h>

using namespace LWF;

void usage(int argc, char *argv[])
{
}


int main(int argc, char *argv[])
{
    LOG_INFO << "Start testing HttpServer" << "\r\n";
    
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

    HttpServer server(&e, addr);
    server.start();
    server.run();

    return 0;
}

