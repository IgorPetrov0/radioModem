#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "usbcore.h"
#include "serialportsdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected slots:
    void updateCOMPorts();
    void settingCOMPort();
    void changeRegister(int number);
    void portConnectSlot();
    void incommingRegisterSlot(unsigned char reg);
    void errorSlot(QString error);


protected:
    usbCore usbPort;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
