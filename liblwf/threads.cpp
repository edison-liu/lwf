
#include <string.h>
#include <time.h>
#include <errno.h>
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

    memset(&cond_, 0, sizeof(cond_));
    rv = pthread_cond_init(&cond_, 0);
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

bool Synchronized::wait()
{
    int rv;

    lock();
    rv = pthread_cond_wait(&cond_, &mutex_);
    if (rv) 
    {
        unlock();
        LOG_ERROR << "Synchronized: wait() failed with result: " << rv << "\r\n";
        return false;
    }

    unlock();
    return true;
}

bool Synchronized::notify()
{
    int rv;

    lock();
    //rv = pthread_cond_signal(&cond_);
    rv = pthread_cond_broadcast(&cond_);
    if (rv) 
    {
        unlock();
        LOG_ERROR << "Synchronized: notify() failed with result: " << rv << "\r\n";
        return false;
    }

    unlock();
    return true;
}

class CallbackHolder: public Runnable
{
public:
	CallbackHolder(Thread::Callback callback, void *argv):
		callback_(callback),
		argv_(argv)
	{
	}

	~CallbackHolder()
	{
	}

	void run()
	{
		callback_(argv_);
	}

private:
	Thread::Callback callback_;
	void    *argv_;
};

Thread::Thread()
{
}

Thread::Thread(Runnable &r)
{
    runnable_ = &r;
}

Thread::~Thread()
{
    join();
}

void Thread::sleep(long ms)
{
    nsleep(ms/1000, (ms%1000)*1000000);
}

void Thread::nsleep(int secs, long nanos)
{
	long s = secs + nanos / 1000000000;
	long n = nanos % 1000000000;

	struct timespec interval, remainder;
	interval.tv_sec = (int)s;
	interval.tv_nsec = n;
	if (nanosleep(&interval, &remainder) == -1) 
    {
		if (errno == EINTR) 
        {
			LOG_ERROR << "Thread: sleep interrupted" << "\r\n";
		}
	}

}

void Thread::start()
{
    pthread_attr_t  attr;

    pthread_attr_init(&attr);

    pthread_create(&tid_, &attr, thread_starter, this);

    pthread_attr_destroy(&attr);
}

void Thread::start(Runnable &r)
{
    pthread_attr_t  attr;

    runnable_ = &r;
    pthread_attr_init(&attr);

    pthread_create(&tid_, &attr, thread_starter, this);

    pthread_attr_destroy(&attr);
}

void Thread::start(Callback cb, void *argv)
{
    CallbackHolder  *holder = new CallbackHolder(cb, argv);
    start(*holder);
}

void Thread::join()
{
    void* retstat;
    pthread_join(tid_, &retstat);
}

void* Thread::thread_starter(void *t)
{
    Thread *thread = reinterpret_cast<Thread *>(t);

    LOG_INFO << "thread_starter is running tid = " << thread->tid_ << "\r\n";
    thread->runnable_->run();
}


class PooledThread : public Runnable
{
public:
    PooledThread();
    ~PooledThread();

    void run();
    void start();
    void start(Runnable &r);
    void join();

    bool idle() const;
    void activate();
    void release();

private:
    Runnable    *runnable_;
    Synchronized    sync_;
    Thread          thread_;
    bool            idle_;
    Synchronized    started_;
    Synchronized    completed_;

};

PooledThread::PooledThread() :
    runnable_(0), idle_(true)
{
}

PooledThread::~PooledThread()
{
}

void PooledThread::run() 
{
    started_.notify();
    while (true) 
    {
        sync_.wait();
        if (runnable_) 
        {
            runnable_->run();
            idle_ = true;
            completed_.notify();
        }
    }
}

void PooledThread::start()
{
    thread_.start(*this);
    started_.wait();
}

void PooledThread::start(Runnable &r)
{
    runnable_ = &r;
    sync_.notify();
}

void PooledThread::join()
{
    completed_.wait();
    thread_.join();
}

bool PooledThread::idle() const
{
    return idle_;
}

void PooledThread::activate()
{
    idle_ = false;
}

void PooledThread::release()
{
    thread_.join();
}

ThreadPool::ThreadPool(int capacity)
    : thread_vec_(0)
{
    for (int i = 0; i < capacity; i++) 
    {
        PooledThread    *p = create_thread();
        thread_vec_.push_back(p);
        p->start();
    }
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::start(Runnable &r)
{
    get_thread()->start(r);
}

void ThreadPool::join_all()
{
    for (ThreadVec::iterator it = thread_vec_.begin(); it != thread_vec_.end(); ++it) 
    {
        (*it)->join();
    }
}

PooledThread* ThreadPool::create_thread()
{
    return new PooledThread();
}

PooledThread* ThreadPool::get_thread()
{
    PooledThread  *p = 0;
    ThreadVec::iterator it;
    for (it = thread_vec_.begin(); !p && it != thread_vec_.end(); ++it) 
    {
        if ((*it)->idle()) 
        {
            p = *it;
        }
    }

    if (!p) 
    {
        LOG_ERROR << "Null idle PooledThread" << "\r\n";
        return NULL;
    }

    p->activate();
    return p;
}

}



