#ifndef _LWF_EVENT_H_
#define _LWF_EVENT_H_

#include <string>

namespace LWF
{

class Event
{
public:
    Event(std::string name) : name_(name) {}
    virtual ~Event() {}

    std::string name()
    {
        return name_;
    }

protected:
    std::string name_;
};

}

#endif // _LWF_EVENT_H_

