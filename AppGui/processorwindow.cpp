
#include "processorwindow.h"
#include "ui_processorwindow.h"

#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <cstdio>
#include <cstring>
#include <QTextStream>
#include <iostream>

extern "C"
{
    #include "memory.h"
    #include "asm.h"
    #include "arraylist.h"
    #include "processor.h"
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


const char** split(QString text, int &size){
    QStringList qstrList = text.split('\n');

    const char **buf = (const char **)malloc(qstrList.size() * sizeof(const char *));

    size = qstrList.size();
    for (int i = 0; i < qstrList.size(); ++i)
    {
        QByteArray qarr = qstrList.at(i).toLocal8Bit();

        char *str = new char[qarr.size() + 1];
        strcpy(str, qarr.data());
        std::cout << str << std::endl;
        buf[i] = str;
    }
    return buf;
}

void ProcessorWindow::on_assemblyButton_clicked()
{
    QString asmText = ui->asmTextEdit->toPlainText();

    int length;
    const char** buf = split(asmText, length);


//    const char *buf[] = {
//        "; Program to copy and determine length of string",
//        ".origin 1000",
//        "start: mov #msga, r1",
//        "  mov #msgb, r2",
//        "  clr r0",
//        "l1: movb (r1)+, (r2)+",
//        "  beq done",
//        "  inc r0",
//        "  br l1",
//        "done: halt",
//        "msga: .string \"A string whose length is to be determined\"",
//        "msgb: .string \"Different string that should get overwritten\"",
//        ".end start"
//    };

    assembly(buf, length);

    QString machinCodeStr;

    char* outbuf = (char *)malloc((2*6+2)*arraySize()/2);
    for (int i = 0; i < arraySize(); i+=2){

        uint16_t addr = arrayGetValue(i);
        uint16_t instr = arrayGetValue(i+1);

        std::snprintf(outbuf+(2*6+2)*i/2, 2*6+2, "%o %06o\n", addr, instr);
    }

    for (int i = 0; i < arraySize(); i+=2) {
        machinCodeStr.append(outbuf+(2*6+2)*i/2);
    }

    ui->machineCodeEditor->clear();
    ui->machineCodeEditor->append(machinCodeStr);
}

void ProcessorWindow::on_runButton_clicked()
{
    ui->txtEditor->append(QString("Button 'RUN' is pressed"));
    evalCode();

    QString r0;
    QTextStream(&r0) << "R0: " << *getRegister(0);
    ui->r0label->setText(r0);

    QString r1;
    QTextStream(&r1) << "R1: " << *getRegister(1);
    ui->r1label->setText(r1);

    QString r2;
    QTextStream(&r2) << "R2: " << *getRegister(2);
    ui->r2label->setText(r2);
}


uint16_t sample_programm[] = {
0012701,
0000024,
0012702,
0000076,
0005000,
0112122,
0001402,
0005200,
0000774,
0000000,

0020101,
0072163,
0064562,
0063556,
0073440,
0067550,
0062563,
0066040,
0067145,
0072147,
0020150,
0071551,
0072040,
0020157,
0062542,
0062040,
0072145,
0071145,
0064555,
0062556,
0000144,

0064504,
0063146,
0071145,
0067145,
0020164,
0072163,
0064562,
0063556,
0072040,
0060550,
0020164,
0064163,
0072557,
0062154,
0063440,
0072145,
0067440,
0062566,
0073562,
0064562,
0072164,
0067145,
0000000
};

void ProcessorWindow::on_loadButton_clicked()
{
    //temporary, while assembler works wrong
    for (int i = 0; i < sizeof(sample_programm)/sizeof(sample_programm[0]); ++i) {
        arraySetValue(i,sample_programm[i]);
    }

    for (int i = 0; i < arraySize(); i+=2){
        uint8_t *procMemory = getMemory(i);
        uint16_t machineWord = arrayGetValue(i+1);
        //TODO remake, init memory
        *procMemory = machineWord;
    }
}
