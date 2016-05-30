#ifndef _LWF_EVENTHANDLER_H_
#define _LWF_EVENTHANDLER_H_

#include <eventloop.h>

namespace LWF
{

class EventLoop;

class EventHandler
{
public:
    EventHandler(EventLoop *e);
    virtual ~EventHandler();

    virtual int handle_input(int fd);
    virtual int handle_output(int fd);
    virtual int handle_exception(int fd);
    virtual int handle_timeout();

    virtual int get_handler();
    virtual void set_handler(int fd);

    virtual void eventloop(EventLoop *e);
    virtual EventLoop* eventloop();

private:

    EventLoop   *eventloop_;

};

}

#endif // _LWF_EVENTHANDLER_H_

