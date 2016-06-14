#ifndef _LWF_SELECTOR_H_
#define _LWF_SELECTOR_H_

#include <vector>
#include <map>

#include <sys/select.h>

//#include <eventhandler.h>

namespace LWF
{

class EventHandler;

enum IoEventType
{
    IOT_READ,
    IOT_WRITE,
    IOT_EXCEPTION,
    IOT_ACCEPT,
    IOT_CONNECT,
    IOT_DISCONNECT,
    IOT_ANY,
};


typedef void (*IoEventCb)(int, IoEventType);

class SelectorList
{
public:
    SelectorList();
    ~SelectorList();
    
    void add_ioevent(int fd, IoEventType type, IoEventCb &cb);

    int register_handler(EventHandler *eh, IoEventType type);
    int register_handler(int fd, EventHandler *eh, IoEventType type);
    int unregister_handler(int fd, EventHandler *eh, IoEventType type);
    
    void dispatch();

    void dump();

private:
    enum {
        SEL_IDX_RD = 0,
        SEL_IDX_WR = 1,
        SEL_IDX_EX = 2,
        SEL_IDX_MAX = 3,
    };

    struct SelectorNode
    {
        int     fd_;
        bool    inuse_;
        IoEventType type_;
        IoEventCb   cb_[SEL_IDX_MAX];
        IoEventType iot_[SEL_IDX_MAX];
        
        SelectorNode();
        ~SelectorNode();
    };    
        
    std::vector<SelectorNode>   selector_node_;

    std::map<int, EventHandler*>    handler_repos_[SEL_IDX_MAX];

    //std::map<int, EventHandler*>    rd_handlers;
    //std::map<int, EventHandler*>    wr_handlers;
    //std::map<int, EventHandler*>    ex_handlers;
    
    fd_set  fds_[SEL_IDX_MAX];
    fd_set  test_fds_[SEL_IDX_MAX];
    int     max_fd_;
    int     testfds_n_;
    
};
 
}

#endif // _LWF_SELECTOR_H_
