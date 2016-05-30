
#include <iostream>

#include <logging.h>


//using namespace LWF;


int main()
{
    //LogMessage(__FILE__, __LINE__);
    LOG_INFO << "Start" << " Continue\n";
    LOG_INFO << 123 << "\n";
    LOG_INFO << "End" << "\n";
    //LOG_FATAL("Start\n");
    //LOG_FATAL("%d\n", 100);
    //LOG_FATAL("%s\n", "test");
    
    return 0;
}

