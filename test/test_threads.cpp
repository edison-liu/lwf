
#include <logging.h>
#include <threads.h>

using LWF::Synchronized;
using LWF::FunctionCallback;
using LWF::Thread;
using LWF::Runnable;
using LWF::ThreadPool;

static Synchronized g_sync;

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
        g_sync.wait();
        LOG_INFO << "TestRunnable2() is running" << "\r\n";
        //LWF::Thread::sleep(1000);
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
        g_sync.notify();
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


class TestProducer : public Runnable
{
public:
    void run();

};

void TestProducer::run()
{
    int n;
    while (n++ < 12) 
    {
        LOG_TRACE << "\tTestProducer::run() is running" << "\r\n";
        g_sync.notify();
        Thread::sleep(2000);
    }
}

class TestConsumer : public Runnable
{
public:
    void run();

};

void TestConsumer::run()
{
    int n;
    while (n++ < 12) 
    {
        g_sync.wait();
        LOG_TRACE << "\tTestConsumer::run() is running" << "\r\n";
    }
}


class TestClassMethod
{
public:
    void member_func();

};

void TestClassMethod::member_func()
{
    LOG_TRACE << "\tTestClassMethod::member_func() is running" << "\r\n";
}


static void test_threadpool()
{
    LOG_INFO << "Start to test ThreadPool()" << "\r\n";

    ThreadPool  thread_pool(16);
    TestProducer    p1, p2, p3, p4;
    TestConsumer    c1;

    //Thread::sleep(1000);
    thread_pool.start(c1);
    Thread::sleep(1000);
    thread_pool.start(p1);

    TestClassMethod tc;
    FunctionCallback<TestClassMethod> fc(tc, &TestClassMethod::member_func); 
    thread_pool.start(fc);
    //thread_pool.start(p2);
    //thread_pool.start(p3);
    //thread_pool.start(p4);

    thread_pool.join_all();

    //while (true) {
    //    Thread::sleep(1000);
    //}

    LOG_INFO << "Finished testing ThreadPool()" << "\r\n";
}

static void run_test()
{
    test_syncronized();

    test_thread();

    test_threadpool();
}

int main(int argc, char *argv[])
{
    LOG_INFO << "Start testing Threads" << "\r\n";

    run_test();

    return 0;
}

