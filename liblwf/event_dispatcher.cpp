
#include <string>
#include <list>
#include <map>

#include <logging.h>
#include <event.h>
#include <eventhandler.h>
#include <event_dispatcher.h>

namespace LWF
{

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

int EventDispatcher::register_handler(const std::string name, EventHandler *handler)
{
    event_handler_map_[name].push_back(handler);
}

int EventDispatcher::register_handler(const char *name, EventHandler *handler)
{
    event_handler_map_[name].push_back(handler);
}

int EventDispatcher::remove_handler(const char *name, EventHandler *handler)
{
    std::string event_name(name);
    EventHandlerList::iterator  it;

    for (it = event_handler_map_[event_name].begin(); it != event_handler_map_[event_name].end(); ++it) 
    {
        //(*it)->handle_event(e);
        if ((*it)->equals(handler)) 
        {
            event_handler_map_[event_name].erase(it);
            return true;
        }
    }

}

int EventDispatcher::dispatch(Event *e)
{
    EventHandlerList::iterator  it;

    std::string event_name = e->name();

    for (it = event_handler_map_[event_name].begin(); it != event_handler_map_[event_name].end(); ++it) 
    {
        (*it)->handle_event(e);
    }
}

}

