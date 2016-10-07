#ifndef PROCESSORWINDOW_H
#define PROCESSORWINDOW_H

#include <QMainWindow>

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
    void slotRunButtonClicked();
    void slotStopButtonClicked();
    void slotResetButtonClicked();

private:
    QGraphicsScene *gScene;
    QImage *gImage;

    Ui::ProcessorWindow *ui;

    const uint16_t monitorHeight;
    const uint16_t monitorWidth;
};

#endif // PROCESSORWINDOW_H
