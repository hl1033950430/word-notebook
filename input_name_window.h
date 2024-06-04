#ifndef INPUTNAMEWINDOW_H
#define INPUTNAMEWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "mainwindow.h"

namespace Ui {
class InputNameWindow;
}

class InputNameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InputNameWindow(QWidget *parent = nullptr);
    ~InputNameWindow();

private slots:
    void confirmNotebookName();

private:
    Ui::InputNameWindow *ui;
};

#endif // INPUTNAMEWINDOW_H
