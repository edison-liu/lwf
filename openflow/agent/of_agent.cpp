#include <sys/types.h> 
#include <sys/socket.h>

#include <logging.h>
#include <sockaddr.h>
#include <eventloop.h>

#include <of_agent.h>
#include <of_connection.h>

namespace LWF
{

namespace OPENFLOW
{

//using LWF::SockAddr;
//using namespace LWF;

OfAgent::OfAgent() :
    controller_addr_(0)
{
    //event_loop_ = new LWF::EventLoop;
}

OfAgent::OfAgent(LWF::SockAddr &addr) :
    controller_addr_(0)
{
    connector_ = new LWF::Connector<OFConnection>(&event_loop_);
}

OfAgent::~OfAgent()
{
}

void OfAgent::start()
{
    
}

void OfAgent::start(LWF::SockAddr &addr)
{
    controller_addr_ = addr;

    connector_->start(controller_addr_);
}

void OfAgent::run()
{
    while (true) 
    {
        event_loop_.run();
    }
}

}

}

