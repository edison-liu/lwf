#include <iostream>

#include <logging.h>
#include <event_dispatcher.h>
#include <event.h>
#include <eventhandler.h>

using namespace LWF;

class PrintNameEventHandler : public EventHandler
{
public:
    int handle_event(Event *e)
    {
        LOG_TRACE << "PrintNameEventHandler::handle_event()" << "\r\n";
        LOG_TRACE << "    e->name() = " << e->name() << "\r\n";
    }

};


static void test1()
{
    LOG_INFO << "Start testing test1" << "\r\n";

    EventDispatcher     ed;
    Event   e1("test_event1");
    PrintNameEventHandler    eh1;
    EventHandler    eh2;

    std::string str("test_event1");
    ed.register_handler(str, &eh2);
    ed.register_handler("test_event1", &eh2);
    ed.register_handler("test_event1", &eh1);
    ed.dispatch(&e1);

    ed.remove_handler("test_event1", &eh2);
    ed.dispatch(&e1);

    ed.remove_handler("test_event1", &eh1);
    ed.dispatch(&e1);

    ed.remove_handler("test_event1", &eh2);
    ed.dispatch(&e1);
}

static void test2()
{
    LOG_INFO << "Start testing test2" << "\r\n";

    EventDispatcher     ed;
    Event   e1("test_event1");
    Event   e2("test_event2");
    PrintNameEventHandler    eh1;
    EventHandler    eh2;

    //ed.register_handler("test_event2", &eh2);
    LOG_INFO << "Register event1" << "\r\n";
    ed.register_handler("test_event1", &eh1);
    ed.dispatch(&e1);
    ed.dispatch(&e2);

    LOG_INFO << "Register event2" << "\r\n";
    ed.register_handler("test_event2", &eh2);
    //ed.register_handler("test_event1", &eh1);
    //ed.remove_handler("test_event1", &eh2);
    ed.dispatch(&e1);
    ed.dispatch(&e2);

    LOG_INFO << "Remove event1" << "\r\n";
    ed.remove_handler("test_event1", &eh1);
    ed.dispatch(&e1);
    ed.dispatch(&e2);

    LOG_INFO << "Remove event2" << "\r\n";
    ed.remove_handler("test_event2", &eh2);
    ed.dispatch(&e1);
    ed.dispatch(&e2);
}


int main()
{
    LOG_INFO << "Start testing EventDispatcher" << "\r\n";

    test1();
    test2();

    return 0;
}

