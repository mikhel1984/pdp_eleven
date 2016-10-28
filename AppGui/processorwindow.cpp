
#include "processorwindow.h"
#include "ui_processorwindow.h"

#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <cstdio>
#include <cstring>

extern "C"
{
    #include "memory.h"
    #include "asm.h"
}

ProcessorWindow::ProcessorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessorWindow)
    , monitorHeight(VIDEO_HEIGHT)
    , monitorWidth(VIDEO_WIDTH)
{
    ui->setupUi(this);
    gScene = new QGraphicsScene();    
    gImage = new QImage(monitorWidth, monitorHeight, QImage::Format_Mono);
    ui->monitor->setScene(gScene);

    ui->txtEditor->setText(QString("Press 'RUN' button"));
}

ProcessorWindow::~ProcessorWindow()
{
    delete gScene;
    delete gImage;

    delete ui;
}

/*
 *  Public slot
 */

void ProcessorWindow::slotRunButtonClicked()
{
    ui->txtEditor->append(QString("Button 'RUN' is pressed"));
}

void ProcessorWindow::slotStopButtonClicked()
{
    ui->txtEditor->append(QString("Button 'STEP' is pressed"));
}

void ProcessorWindow::slotResetButtonClicked()
{
    ui->txtEditor->append(QString("Button 'RESET' is pressed"));
}

void ProcessorWindow::on_assemblyButton_clicked()
{
    QString asmText = ui->assemblyEditor->toPlainText();
    char* t;
    const char* asmBuffer = asmText.toLocal8Bit().constData();
    int size = 0;

    int lines = 0;
    for (char* counter = (char *)asmBuffer; *counter != '\0'; counter++){
        if(*counter=='\n')
            lines++;
    }

    char *buf[lines];
    char* pch = NULL;

    pch = strtok((char *)asmBuffer, "\n");

    int li = 0;
    while (pch != NULL)
    {
        printf("%s\n", pch);
        buf[li++] = pch;
        pch = strtok(NULL, "\n");
    }

//    char *buf[] = {
//        "; Program to copy and determine length of string",
//        ".origin 1000",
//        "start: mov #msga, r1",
//        "   mov #msgb, r2",
//        "   clr r0",
//        "l1: movb (r1)+, (r2)+",
//        "   beq done",
//        "   inc r0",
//        "   br l1",
//        "done: halt",
//        "msga: .string \"A string whose length is to be determined\"",
//        "msgb: .string \"Different string that should get overwritten\"",
//        ".end start"
//    };

    uint16_t *result = NULL;
    uint16_t resultSize = 0;

    assembly(buf,13,&result,&resultSize);

//    ui->machineCodeEditor->append(QString::number(result[0]));
//    ui->machineCodeEditor->append(result);

    QString machinCodeStr;

//    char outbuf[(2*6+1)];
//      char outbuf[(2*6+1)*resultSize];
    char* outbuf = (char *)malloc((2*6+2)*resultSize/2);
//    char outbuf[20];

    for (int i = 0; i < resultSize; i+=2) {
//        std::snprintf(outbuf, sizeof outbuf, "%o%o", result[i], result[i+1]);

//        std::snprintf(&outbuf[i*6], sizeof outbuf, "%o", result[i]);
        std::snprintf(outbuf+(2*6+2)*i/2, 2*6+2, "%o %o\n", result[i],result[i+1]);
//        machinCodeStr.append(outbuf);
//        machinCodeStr.append('\n');
    }

//    QString::from
//    machinCodeStr.append(new QString(result));

    for (int i = 0; i < resultSize; i+=2) {
        machinCodeStr.append(outbuf+(2*6+2)*i/2);
    }

    ui->machineCodeEditor->append(machinCodeStr);
}
