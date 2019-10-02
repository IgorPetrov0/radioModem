#ifndef SERIALPORTSDIALOG_H
#define SERIALPORTSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QSerialPort>

namespace Ui {
class serialPortsDialog;
}

class serialPortsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit serialPortsDialog(QWidget *parent);
    ~serialPortsDialog();
    void setPort(QSerialPort *port);


protected:
    QSerialPort *port1;
    void fillComboBoxes();

protected slots:
    void saveSlot();


private:
    Ui::serialPortsDialog *ui;
};

#endif // SERIALPORTSDIALOG_H
