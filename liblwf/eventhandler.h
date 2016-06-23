#ifndef _LWF_EVENTHANDLER_H_
#define _LWF_EVENTHANDLER_H_

#include <eventloop.h>

namespace LWF
{

class EventLoop;
class Event;

class EventHandler
{
public:
    EventHandler(EventLoop *e);
    EventHandler();
    virtual ~EventHandler();

    virtual int handle_input(int fd);
    virtual int handle_output(int fd);
    virtual int handle_exception(int fd);
    virtual int handle_timeout();

    virtual int handle_event(Event *e);

    bool equals(const EventHandler *e) const;

    virtual int get_handler();
    virtual void set_handler(int fd);

    virtual void eventloop(EventLoop *e);
    virtual EventLoop* eventloop();

private:

    EventLoop   *eventloop_;

};


template <class C>
class ServiceHandler : public EventHandler
{
public:
    typedef int (C::*Callback)(int);

    ServiceHandler(EventLoop *e) : 
        EventHandler(e), pTarget_(0),  inputCb_(0), outputCb_(0), exceptionCb_(0), timeoutCb_(0)
    {
    }

    virtual ~ServiceHandler() {}

    virtual void set_input_callback(C &obj, Callback cb)
    {
        pTarget_ = &obj;
        inputCb_ = cb;
    }

    virtual void set_output_callback(C &obj, Callback cb)
    {
        pTarget_ = &obj;
        outputCb_ = cb;
    }

    virtual void set_exception_callback(C &obj, Callback cb)
    {
        pTarget_ = &obj;
        exceptionCb_ = cb;
    }

    virtual void set_timeout_callback(C &obj, Callback cb)
    {
        pTarget_ = &obj;
        timeoutCb_ = cb;
    }

    virtual int handle_input(int fd)
    {
        if (pTarget_) 
        {
            return (pTarget_->*inputCb_)(fd);
        }
    }

    virtual int handle_output(int fd)
    {
        if (pTarget_) 
        {
            return (pTarget_->*outputCb_)(fd);
        }
    }

    virtual int handle_exception(int fd)
    {
        if (pTarget_) 
        {
            return (pTarget_->*exceptionCb_)(fd);
        }
    }

    virtual int handle_timeout()
    {
        if (pTarget_) 
        {
            return (pTarget_->*timeoutCb_)(get_handler());
        }
    }

private:
    C           *pTarget_;
    Callback    inputCb_;
    Callback    outputCb_;
    Callback    exceptionCb_;
    Callback    timeoutCb_;
};



}

#endif // _LWF_EVENTHANDLER_H_

