#include "processorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProcessorWindow w;
    w.show();

    return a.exec();
}
