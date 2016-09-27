#include "processorwindow.h"
#include "ui_processorwindow.h"

#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QTimer>

#define SCREEN_SIZE 256
#define STEP 16
#define UPDATE_TIME 100

ProcessorWindow::ProcessorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProcessorWindow)
{
    ui->setupUi(this);
    gScene = new QGraphicsScene();    
    gImage = new QImage(SCREEN_SIZE, SCREEN_SIZE, QImage::Format_Mono);
    ui->monitor->setScene(gScene);

    step = 1;
    ui->txtEditor->setText(QString("Press 'RUN' button"));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_monitor()));
}

ProcessorWindow::~ProcessorWindow()
{
    delete gScene;
    delete gImage;
    delete timer;

    delete ui;
}

void ProcessorWindow::on_runButton_clicked()
{
    if(!timer->isActive())
        timer->start(UPDATE_TIME);
    ui->txtEditor->append(QString("Button 'RUN' is pressed"));
}

void ProcessorWindow::on_stopButton_clicked()
{
    if(timer->isActive())
        timer->stop();
    nextStep();
    redrawMonitor();
    ui->txtEditor->append(QString("Button 'STEP' is pressed"));
}

void ProcessorWindow::on_resetButton_clicked()
{
    if(timer->isActive())
        timer->stop();
    ui->txtEditor->append(QString("Button 'RESET' is pressed"));
}

void ProcessorWindow::update_monitor() {
    redrawMonitor();
    nextStep();
}

void ProcessorWindow::redrawMonitor() {

    for(int i = 0; i < SCREEN_SIZE; ++i) {
        for(int j = 0; j < SCREEN_SIZE; ++j) {
            gImage->setPixel(i, j, (i+j + step)%STEP > 0 ? 1 : 0);
        }
    }

    gScene->clear();
    gScene->addPixmap(QPixmap::fromImage(*gImage));

    //ui->monitor->setScene(gScene);
}

void ProcessorWindow::nextStep() {
    if(step < STEP)
        ++step;
    else
        step = 1;
}
