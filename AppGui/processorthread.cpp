#include "ProcessorThread.h"

extern "C"
{
    #include "processor.h"
}

void ProcessorThread::process()
{
    prepareProcessor();

    //setProgrammStart(01000 + 012);
    setProgrammStart(01000);
    setProgrammStack(06543);

    evalCode();






}

void ProcessorThread::stop()
{
}

