
#include <logging.h>
#include <event.h>
#include <eventhandler.h>

using namespace LWF;

namespace LWF
{

EventHandler::EventHandler()
{
}

EventHandler::EventHandler(EventLoop *e)
    : eventloop_(e)
{
}

EventHandler::~EventHandler()
{
}

int EventHandler::handle_input(int fd)
{
    LOG_TRACE << "EventHandler::handle_input()" << "\r\n";
    return -1;
}

int EventHandler::handle_output(int fd)
{
    LOG_TRACE << "EventHandler::handle_output()" << "\r\n";
    return -1;
}

int EventHandler::handle_exception(int fd)
{
    LOG_TRACE << "EventHandler::handle_exception()" << "\r\n";
    return -1;
}

int EventHandler::handle_timeout()
{
    LOG_TRACE << "EventHandler::handle_timeout()" << "\r\n";
    return -1;
}

int EventHandler::handle_event(Event *e)
{
    LOG_TRACE << "EventHandler::handle_event()" << "\r\n";
    LOG_TRACE << "    e->name() = " << e->name() << "\r\n";
    return -1;
}

bool EventHandler::equals(const EventHandler *e) const
{
    return (this == e);
}

int EventHandler::get_handler()
{
    LOG_TRACE << "EventHandler::get_handler()" << "\r\n";
    return -1;
}

void EventHandler::set_handler(int fd)
{
    LOG_TRACE << "EventHandler::set_handler()" << "\r\n";
}

void EventHandler::eventloop(EventLoop *e)
{
    LOG_TRACE << "EventHandler::eventloop()" << "\r\n";
    eventloop_ = e;
}

EventLoop* EventHandler::eventloop()
{
    LOG_TRACE << "EventHandler::eventloop()" << "\r\n";
    return eventloop_;
}

}

