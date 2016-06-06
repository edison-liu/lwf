#ifndef _LWF_THREADS_H_
#define _LWF_THREADS_H_

#include <vector>

#include <pthread.h>

namespace LWF
{

class Synchronized
{
public:
    Synchronized();
    ~Synchronized();

    bool lock();
    bool unlock();

    bool wait();
    bool notify();

private:
    pthread_cond_t      cond_;
    pthread_mutex_t     mutex_;

};

class Lock
{
public:
    Lock(Synchronized &s) : sync_(s) { sync_.lock(); }
    ~Lock() { sync_.unlock(); }

private:
    Synchronized    &sync_;
};

class ScopedLock
{
public:
    ScopedLock(Synchronized &s) : sync_(s) { sync_.lock(); }
    ~ScopedLock() { sync_.unlock(); }

private:
    Synchronized    &sync_;
};

class Runnable
{
public:
    Runnable() {}
    ~Runnable() {}

    virtual void run() = 0;

};

template<class C>
class FunctionCallback : public Runnable
{
public:
    typedef void (C::*Callback)();

    FunctionCallback(C &obj, Callback cb) : object_(&obj), method_(cb)
    {
    }

    ~FunctionCallback()
    {
    }

    void run()
    {
        (object_->*method_)();
    }

private:
    C           *object_;
    Callback    method_;

};

class Thread
{
public:
    typedef void (*Callback)(void *);

    Thread();
    Thread(Runnable &r);
    ~Thread();

    static void sleep(long ms);
    void start();
    void start(Runnable &r);
    void start(Callback cb, void* argv = 0);

    template<class Functor>
    void startFunc(Functor fn)
    {
        FunctorRunnable<Functor> *f = new FunctorRunnable<Functor>(fn);
        start(*f);
    }

    void join();
    
protected:

    template<class Functor>
    class FunctorRunnable : public Runnable
    {
    public:
        FunctorRunnable(const Functor &functor) :
            functor_(functor)
        {
        }

        ~FunctorRunnable()
        {
        }

        void run()
        {
            functor_();
        }

    private:
        Functor     functor_;
    };

    static void* thread_starter(void *t);

private:
    pthread_t   tid_;
    Runnable    *runnable_;

    static void nsleep(int secs, long nanos);
};

class PooledThread;

class ThreadPool
{
public:
    ThreadPool(int capacity);
    ~ThreadPool();

    void start(Runnable &r);

    void join_all();

protected:
    PooledThread* get_thread();
    PooledThread* create_thread();

private:
    typedef std::vector<PooledThread *> ThreadVec;

    ThreadVec   thread_vec_;
    Synchronized    sync_;
};

class Task
{
public:

private:
};

class TaskManager
{
public:

private:
};

}

#endif // _LWF_THREADS_H_

