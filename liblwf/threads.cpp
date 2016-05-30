
#include <string.h>
#include <time.h>
#include <threads.h>

#include <logging.h>

namespace LWF
{

Synchronized::Synchronized()
{
    int rv;
    pthread_mutexattr_t attr;

    rv = pthread_mutexattr_init(&attr);
    if (rv) 
    {
        LOG_ERROR << "Constructing Synchronized error with result: " << rv << "\r\n";
    }

    rv = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if (rv) 
    {
        LOG_ERROR << "Constructing Synchronized error with result: " << rv << "\r\n";
    }

    memset(&mutex_, 0, sizeof(mutex_));

    rv = pthread_mutex_init(&mutex_, &attr);
    if (rv) 
    {
        LOG_ERROR << "Constructing Synchronized error with result: " << rv << "\r\n";
    }
    rv = pthread_mutexattr_destroy(&attr);
    if (rv) 
    {
        LOG_ERROR << "Constructing Synchronized error with result: " << rv << "\r\n";
    }
}

Synchronized::~Synchronized()
{
    int rv;

    rv = pthread_mutex_destroy(&mutex_);
    if (rv) 
    {
        LOG_ERROR << "Destructing Synchronized error with result: " << rv << "\r\n";
    }
}

bool Synchronized::lock()
{
    int rv;

    rv = pthread_mutex_lock(&mutex_);
    if (rv) 
    {
        LOG_ERROR << "Synchronized: lock failed with result: " << rv << "\r\n";
        return false;
    }

    return true;
}

bool Synchronized::unlock()
{
    int rv;

    rv = pthread_mutex_unlock(&mutex_);
    if (rv) 
    {
        LOG_ERROR << "Synchronized: unlock failed with result: " << rv << "\r\n";
        return false;
    }

    return true;
}


Thread::Thread()
{
}

Thread::Thread(Runnable &r)
{
    runnable_ = &r;
}

Thread::~Thread()
{
}

void Thread::sleep(long ms)
{
    usleep(ms);
}

void Thread::start()
{
}

void Thread::start(Runnable &r)
{
}

void Thread::join()
{
}


}

