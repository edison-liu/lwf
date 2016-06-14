#ifndef _LWF_EVENTLOOP_H_
#define _LWF_EVENTLOOP_H_

#include <selector.h>
#include <timer.h>
#include <eventhandler.h>

namespace LWF
{

class EventHandler;
//class SelectorList;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    
    void run();
    
    void new_periodic_ms(int ms, TimerCallback& cb);
    
    void add_ioevent(int fd, IoEventType type, IoEventCb cb);

    int register_handler(EventHandler *eh, IoEventType type);
    int register_handler(int fd, EventHandler *eh, IoEventType type);

    int unregister_handler(int fd, EventHandler *eh, IoEventType type);

private:
    TimerList       timer_list_;
    SelectorList    selector_list_;
    
};

}

#endif // _LWF_EVENTLOOP_H_
