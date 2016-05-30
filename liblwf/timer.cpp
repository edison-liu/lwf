
#include <time.h>

#include <logging.h>
#include <timer.h>

using namespace LWF;

namespace LWF
{

TimerNode::TimerNode(time_t t, TimerCallback &cb)
    : expires_(t), cb_(cb)
{}

TimerNode::~TimerNode()
{}

void TimerNode::expire()
{
    cb_();
}

TimerList::TimerList()
{
    
} 

TimerList::~TimerList()
{
    
}

void TimerList::run()
{
    std::list<TimerNode *>::iterator it;
    
    for (it = list_.begin(); it != list_.end(); ++it)
    {
        (*it)->expire();
    }
}

void TimerList::new_periodic_ms(int ms, TimerCallback &cb)
{
    TimerNode *n = new TimerNode(ms, cb);
    
    list_.push_back(n);
}
 
}

