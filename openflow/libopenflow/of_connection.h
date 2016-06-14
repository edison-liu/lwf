#ifndef _OF_CONNECTION_H_
#define _OF_CONNECTION_H_

#include <eventhandler.h>
#include <connector.h>

namespace LWF
{

namespace OPENFLOW
{

class OFConnection
{
public:
    OFConnection(EventLoop *e, int fd);
    virtual ~OFConnection();

    virtual int receive(char *buf, int len);
    virtual int transmit(char *buf, int len);

    int handle_input(int fd);
    int handle_output(int fd);

private:
    int         fd_;
    EventLoop   *eventloop_;

    ServiceHandler<OFConnection>    *conn_;

};

}

}

#endif // _OF_CONNECTION_H_

