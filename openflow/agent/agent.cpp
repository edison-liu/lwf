
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <logging.h>
#include <sockaddr.h>

#include <of_agent.h>

using LWF::SockAddr;
using LWF::OPENFLOW::OfAgent;


static void usage(int argc, char *argv[])
{
    std::cerr << "Usage: " << argv[0] << "<Controller IP> <Port>" << std::endl;
}

int main(int argc, char *argv[])
{
    LOG_INFO << "OpenFlow Agent start to run ..." << "\r\n";

    struct sockaddr_in  addr;

    if (argc != 3) 
    {
        usage(argc, argv);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], &addr.sin_addr) < 0) 
    {
        std::cerr << "Invalid IP address: " << argv[1] << std::endl;
        usage(argc, argv);
        return -1;
    }

    SockAddr sockaddr(addr);

    OfAgent agent;

    agent.start(sockaddr);
    agent.run();

    return 0;
}


