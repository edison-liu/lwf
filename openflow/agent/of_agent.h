#ifndef _OF_AGENT_H
#define _OF_AGENT_H

//#include <sockaddr.h>
//#include <eventloop.h>
#include <connector.h>

#include <of_connection.h>

namespace LWF
{

namespace OPENFLOW
{

class SockAddr;
class EventLoop;
//class Connector;
//class OFConnection;

class OfAgent
{
public:
    OfAgent();
    OfAgent(LWF::SockAddr &addr);
    ~OfAgent();

    void start();
    void start(LWF::SockAddr &addr);
    void run();

protected:


private:

    int              fd_;
    //LWF::SockAddr    agent_addr_;
    LWF::SockAddr    controller_addr_;
    LWF::EventLoop   event_loop_;

    LWF::Connector<OFConnection>    *connector_;

};

}

}

#endif // _OF_AGENT_H

