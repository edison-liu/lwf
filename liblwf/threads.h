#ifndef _LWF_THREADS_H_
#define _LWF_THREADS_H_

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

private:
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

class ScopeLock
{
public:

private:
};

class Runnable
{
public:
    Runnable() {}
    ~Runnable() {}

    virtual void run() = 0;

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

class ThreadPool
{
public:

private:
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

