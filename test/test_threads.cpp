
#include <logging.h>
#include <threads.h>


static void test_syncronized()
{
    LOG_INFO << "Testing Synchronized()" << "\r\n";

    LWF::Synchronized    s;
    s.lock();
    s.unlock();

}

static void run_test()
{
    test_syncronized();
}

int main(int argc, char *argv[])
{
    LOG_INFO << "Start testing Threads" << "\r\n";

    run_test();

    return 0;
}

