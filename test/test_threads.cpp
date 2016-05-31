
#include <logging.h>
#include <threads.h>

using LWF::Thread;
using LWF::Runnable;

class TestRunnable2 : public Runnable
{
public:
    virtual void run();
};

void TestRunnable2::run()
{
    int n = 0;
    while (n++ < 3) 
    {
        LOG_INFO << "TestRunnable2() is running" << "\r\n";
        LWF::Thread::sleep(1000);
    }
}

class TestRunnable : public Runnable
{
public:
    virtual void run();

};

void TestRunnable::run()
{
    int n = 0;
    while (n++ < 3) 
    {
        LOG_INFO << "TestRunnable() is running" << "\r\n";
        LWF::Thread::sleep(1000);
    }
}


static void free_func(void*)
{
    LOG_INFO << "free_func() is running" << "\r\n";
}

class FunctorClass
{
public:
    FunctorClass(int cnt = 0) :
        cnt_(cnt)
    {
    }

    ~FunctorClass()
    {
    }

    void test()
    {
        LOG_INFO << "FunctorClass::test() is running. cnt_ = " << cnt_ << "\r\n";
    }

    static void thread_starter()
    {
        //test();
    }

private:
    int cnt_;
};


class Functor
{
public:
	void operator () ()
	{
		LOG_INFO << "Functor operator() is running" << "\r\n";
	}
};

static void test_thread()
{
    LOG_INFO << "Start to test Thread()" << "\r\n";

    TestRunnable    tr;
    TestRunnable2    tr2;
    Thread          t;
    Thread          t2;

    t.start(tr);
    t2.start(tr2);
    Thread::sleep(5);
    t.join();
    t2.join();

    Thread  t3;
    t3.start(free_func);

    Thread t4;
    //FunctorClass f(100);
    //t4.startFunc(&FunctorClass::test);
    //t4.startFunc(&f.test);
    t4.startFunc(Functor());

    LOG_INFO << "Finished testing Thread()" << "\r\n";
}

static void test_syncronized()
{
    LOG_INFO << "Start to test Synchronized()" << "\r\n";

    LWF::Synchronized    s;
    s.lock();
    s.unlock();

    LOG_INFO << "Finished testing Synchronized()" << "\r\n";
}

static void run_test()
{
    test_syncronized();

    test_thread();
}

int main(int argc, char *argv[])
{
    LOG_INFO << "Start testing Threads" << "\r\n";

    run_test();

    return 0;
}

