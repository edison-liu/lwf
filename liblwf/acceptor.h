#ifndef _LWF_ACCEPTOR_H_
#define _LWF_ACCEPTOR_H_



namespace LWF
{

class EventLoop;
class EventHandler;
class SockAddr;
class TcpConnection;

class Acceptor : public EventHandler
{
public:
    Acceptor(EventLoop *e, SockAddr &a);
    virtual ~Acceptor();

    virtual int start();
    virtual int start(const SockAddr &a);
    virtual int close();

    virtual int handle_input(int fd);
    virtual int get_handler();
    virtual void set_handler(int fd);

    // To be override by concrete subclass
    virtual TcpConnection* new_connection(EventLoop *e, int fd);

private:
    EventLoop   *event_loop_;
    int         fd_;
    SockAddr    addr_;

    int listen_i();
    void onConnection();

};

}

#endif // _LWF_ACCEPTOR_H_

