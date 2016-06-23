#ifndef _LWF_EVENT_DISPATCHER_H_
#define _LWF_EVENT_DISPATCHER_H_

#include <list>
#include <map>

namespace LWF
{

class Event;
class EventHandler;

class EventDispatcher
{
public:
    EventDispatcher();
    virtual ~EventDispatcher();

    int register_handler(const std::string name, EventHandler *handler);
    int register_handler(const char *name, EventHandler *handler);

    int remove_handler(const char *name, EventHandler *handler);

    int dispatch(Event *e);

protected:
    typedef std::list<EventHandler*>    EventHandlerList;
    typedef std::map<std::string, EventHandlerList>    EventHandlerMap;

    EventHandlerMap     event_handler_map_;

};

}

#endif // _LWF_EVENT_DISPATCHER_H_

