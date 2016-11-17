#include "processorthread.h"

extern "C"
{
    #include "process.h"
}

void ProcessorThread::process()
{
    evalCode();
}

void ProcessorThread::stop()
{
}

