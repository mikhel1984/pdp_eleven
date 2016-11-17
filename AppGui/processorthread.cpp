#include "processorthread.h"

extern "C"
{
    #include "processor.h"
}

void ProcessorThread::process()
{
    evalCode();
}

void ProcessorThread::stop()
{
}

