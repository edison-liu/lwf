#ifndef _LWF_CONNECTOR_H
#define _LWF_CONNECTOR_H

#include <unistd.h>
#include <fcntl.h>

#include <eventhandler.h>
#include <sockaddr.h>

namespace LWF
{

class EventHandler;
class EventLoop;
class SockAddr;

template <class T>
class Connector
{
public:
    Connector();
    Connector(EventLoop *e);
    virtual ~Connector();

    virtual int start(SockAddr &svc_addr);
    virtual int close();

    virtual int on_connection(int fd);

    virtual void set_handler(int fd);
    virtual int get_handler();

    virtual void set_eventloop(EventLoop *e);

    virtual void register_connector(EventLoop *e);
    virtual void unregister_connector(EventLoop *e);

protected:
    virtual T* new_svc_handler();

private:
    int fd_;
    EventLoop   *eventloop_;
    ServiceHandler<Connector>   *service_handler_;
    //T*          conn_;

};


template <class T>
Connector<T>::Connector()
{
}

template <class T>
Connector<T>::Connector(EventLoop *e) :
    eventloop_(e)
{
    //ServiceHandler<Connector>   sh(e);
    //sh.set_input_callback(*this, &Connector::on_connection);
}

template <class T>
Connector<T>::~Connector()
{
}

template <class T>
int Connector<T>::start(SockAddr &svc_addr)
{
    int backlog = 5;
    int rv = 0;
    
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0)
    {
        LOG_ERROR << "socket error." << "\r\n";
    }

    // Set non-blocking
    int flag = fcntl(fd_, F_GETFL, 0);
    rv = fcntl(fd_, F_SETFL, flag | O_NONBLOCK);
    if (rv < 0)
    {
        LOG_ERROR << "fcntl error " << rv << "\r\n";
    }

    //LOG_TRACE << "Connecting to# " << inet_ntoa((svc_addr.addr())->sin_addr) << ":" 
    //          << ntohs((svc_addr.addr())->sin_port) << "\r\n";
    rv = connect(fd_, (struct sockaddr *)svc_addr.addr(), svc_addr.length()); 
    if (rv < 0)
    {
        LOG_ERROR << "connect error " << rv << "\r\n";
    }
    
    register_connector(eventloop_);

    return 0;
}

template <class T>
int Connector<T>::close()
{
}

template <class T>
int Connector<T>::on_connection(int fd)
{
    LOG_TRACE << "Connector: on_connection for fd " << fd << "\r\n";
    unregister_connector(eventloop_);
    new_svc_handler();
}

template <class T>
void Connector<T>::set_handler(int fd)
{
    fd_ = fd;
}

template <class T>
int Connector<T>::get_handler()
{
    return fd_;
}

template <class T>
T* Connector<T>::new_svc_handler()
{
    return new T(eventloop_, fd_);
}

template <class T>
void Connector<T>::set_eventloop(EventLoop *e)
{
    eventloop_ = e;
}

template <class T>
void Connector<T>::register_connector(EventLoop *e)
{
    service_handler_ = new ServiceHandler<Connector>(e);
    service_handler_->set_output_callback(*this, &Connector::on_connection);

    eventloop_->register_handler(fd_, service_handler_, IOT_WRITE);
}

template <class T>
void Connector<T>::unregister_connector(EventLoop *e)
{
    eventloop_->unregister_handler(fd_, service_handler_, IOT_WRITE);
    delete service_handler_;
}

}

#endif // _LWF_CONNECTOR_H

