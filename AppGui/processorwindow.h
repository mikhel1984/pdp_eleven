#ifndef PROCESSORWINDOW_H
#define PROCESSORWINDOW_H

#include <QMainWindow>
#include <stdint.h>

class QGraphicsScene;
class QImage;

namespace Ui {
class ProcessorWindow;
}

class ProcessorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProcessorWindow(QWidget *parent = 0);
    ~ProcessorWindow();

private:

private slots:
    void on_assemblyButton_clicked();

    void on_runButton_clicked();

private:
    QGraphicsScene *gScene;
    QImage *gImage;

    Ui::ProcessorWindow *ui;

    const uint16_t monitorHeight;
    const uint16_t monitorWidth;
};

#endif // PROCESSORWINDOW_H
