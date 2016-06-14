
#include <logging.h>
#include <eventloop.h>

using namespace LWF;

namespace LWF
{

EventLoop::EventLoop()
{
    //LOG_TRACE << "Constructor of EventLoop" << "\r\n";
}

EventLoop::~EventLoop()
{
    //LOG_TRACE << "Destructor of EventLoop" << "\r\n";
}

void EventLoop::run()
{
    timer_list_.run();
    
    selector_list_.dispatch();
}

void EventLoop::new_periodic_ms(int ms, TimerCallback& cb)
{
    timer_list_.new_periodic_ms(ms, cb);
}

void EventLoop::add_ioevent(int fd, IoEventType type, IoEventCb cb)
{
    selector_list_.add_ioevent(fd, type, cb);
}

int EventLoop::register_handler(EventHandler *eh, IoEventType type)
{
    selector_list_.register_handler(eh, type);
}

int EventLoop::register_handler(int fd, EventHandler *eh, IoEventType type)
{
    selector_list_.register_handler(fd, eh, type);
}

int EventLoop::unregister_handler(int fd, EventHandler *eh, IoEventType type)
{
    selector_list_.unregister_handler(fd, eh, type);
}

}
