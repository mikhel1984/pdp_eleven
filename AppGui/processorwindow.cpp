
#include "processorwindow.h"
#include "ui_processorwindow.h"

#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QTimer>

extern "C"
{
    #include "memory.h"
}

ProcessorWindow::ProcessorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessorWindow)
    , monitorHeight(getVideoHeight())
    , monitorWidth(getVideoWidth())
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
 *                      Public slot
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
