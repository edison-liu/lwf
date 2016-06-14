
#include <string.h>

#include <logging.h>
#include <selector.h>
#include <eventloop.h>

using namespace LWF;

namespace LWF
{

SelectorList::SelectorNode::SelectorNode()
{}

SelectorList::SelectorNode::~SelectorNode()
{}

SelectorList::SelectorList()
    : max_fd_(0), testfds_n_(0), selector_node_(1024)
{
    for (int i = 0; i < SEL_IDX_MAX; i++)
        FD_ZERO(&fds_[i]);
}

SelectorList::~SelectorList()
{}

void SelectorList::add_ioevent(int fd, IoEventType type, IoEventCb &cb)
{
    selector_node_[fd].fd_ = fd;
    selector_node_[fd].type_ = type;
    selector_node_[fd].cb_[type] = cb;
    selector_node_[fd].inuse_ = true;
    
    if (fd > max_fd_)
        max_fd_ = fd;
    FD_SET(fd, &fds_[type]);
}

int SelectorList::register_handler(EventHandler *eh, IoEventType type)
{
    register_handler(eh->get_handler(), eh, type); 
}


int SelectorList::register_handler(int fd, EventHandler *eh, IoEventType type)
{
    handler_repos_[type].insert(std::make_pair(fd, eh));

    if (fd > max_fd_)
        max_fd_ = fd;
    FD_SET(fd, &fds_[type]);

    LOG_INFO << "Register handler for fd " << fd << " Type: " << type << "\r\n";
}

int SelectorList::unregister_handler(int fd, EventHandler *eh, IoEventType type)
{
    std::map<int, EventHandler*>::iterator it;

    it = handler_repos_[type].find(fd);
    if (it != handler_repos_[type].end()) 
    {
        handler_repos_[type].erase(it);
    }

    if (fd > max_fd_)
        max_fd_ = fd;
    FD_CLR(fd, &fds_[type]);

    LOG_INFO << "UnRegister handler for fd " << fd << " Type: " << type << "\r\n";
}

void SelectorList::dispatch()
{
    memcpy(test_fds_, fds_, sizeof(fds_));
    testfds_n_ = select(max_fd_ + 1, &test_fds_[SEL_IDX_RD], &test_fds_[SEL_IDX_WR], &test_fds_[SEL_IDX_EX], NULL);
    if (testfds_n_ < 0)
    {
        LOG_ERROR << "select error" << "\r\n";
        return;
    }
    LOG_TRACE << "select returns with " << testfds_n_ << " is set."<< "\r\n";

    // Dispatch the event.
    std::map<int, EventHandler *>::iterator it;
    int n = 0;

    for (int i = 0; (n < testfds_n_) and (i < SEL_IDX_MAX); i++) 
    {
        for (it = handler_repos_[i].begin(); (n < testfds_n_) && (it != handler_repos_[i].end()); ++it) 
        {
            if (FD_ISSET(it->first, &test_fds_[i])) 
            {
                LOG_TRACE << "FD " << it->first << " is set for SEL_IDX " << i << "\r\n";
                switch (i) 
                {
                    case SEL_IDX_RD:
                        it->second->handle_input(it->first);
                        break;
                    case SEL_IDX_WR:
                        it->second->handle_output(it->first);
                        break;
                    case SEL_IDX_EX:
                        it->second->handle_exception(it->first);
                        break;
                    default:
                        LOG_WARN << "Wrong fd " << it->first << "\r\n";
                        break;
                }
                ++n;
                continue;
            }
        }
    }

#if 0
    int n = 0;
    for (int i = 0; i < 1024 && n < testfds_n_; i++)
    {
        if (selector_node_[i].inuse_ == false)
            continue;

        if (FD_ISSET(selector_node_[i].fd_, &test_fds_[selector_node_[i].type_]))
        {
            selector_node_[i].cb_[selector_node_[i].type_](selector_node_[i].fd_, selector_node_[i].type_);
            n++;
        }
    }

#endif
}

void SelectorList::dump()
{

}

}
