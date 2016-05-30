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
    Thread();
    Thread(Runnable &r);
    ~Thread();

    static void sleep(long ms);
    void start();
    void start(Runnable &r);
    void join();
    

private:
    pthread_t   tid;
    Runnable    *runnable_;
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

