#ifndef PROCESSOR_THREAD_H
#define PROCESSOR_THREAD_H

#include <QObject>

class ProcessorThread: public QObject
{
    Q_OBJECT

public slots:
    void process();
    void stop();

signals:
    void finished();

};

#endif // PROCESSOR_THREAD_H
