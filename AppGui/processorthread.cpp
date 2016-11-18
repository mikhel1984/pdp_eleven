#include "processorthread.h"

extern "C"
{
    #include "processor.h"
}

void ProcessorThread::process()
{
    eval();
}

void ProcessorThread::stop()
{
}


