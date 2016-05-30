#ifndef _LWF_TCPCONNECTION_H_
#define _LWF_TCPCONNECTION_H_

namespace LWF
{

class EventHandler;
class EventLoop;

class TcpConnection : public EventHandler
{
public:
    TcpConnection(EventLoop *el);
    TcpConnection(EventLoop *el, int fd);
    virtual ~TcpConnection();

    virtual int handle_input(int fd);
    virtual int get_handler();
    virtual void set_handler(int fd);

    virtual int receive(char *buf, int len);
    virtual int transmit(char *buf, int len);

    virtual void onMessage();

private:
    EventLoop   *event_loop_;
    int         fd_;

};

}

#endif // _LWF_TCPCONNECTION_H_

