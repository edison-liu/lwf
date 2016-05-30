#ifndef _LWF_TIMER_H_
#define _LWF_TIMER_H_

#include <list>

#include <time.h>

namespace LWF
{

typedef void (TimerCallback)();

class TimerNode
{
public:
    TimerNode(time_t t, TimerCallback &cb);
    ~TimerNode();
    
    void expire();

private:
    time_t          expires_;
    time_t          when_;
    TimerCallback   &cb_;
};

class TimerList
{
public:
    TimerList();
    ~TimerList();

    void run();
    
    void new_periodic_ms(int ms, TimerCallback &cb);

private:
    std::list<TimerNode *>   list_;

}; 
 
}

#endif // _LWF_TIMER_H_
