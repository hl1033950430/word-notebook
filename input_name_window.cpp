#include "input_name_window.h"
#include "ui_inputnamewindow.h"

InputNameWindow::InputNameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputNameWindow)
{
    ui->setupUi(this);

    connect(ui->confirmButton, &QPushButton::clicked, this, &InputNameWindow::confirmNotebookName);
}

InputNameWindow::~InputNameWindow()
{
    delete ui;
}

void InputNameWindow::confirmNotebookName()
{
    const QString &name = ui->lineEdit->text();
    MainWindow *father = (MainWindow*)parent();
    const QStringList &nameList = father->getNotebookList();

    if (name == "") {
        QMessageBox::warning(this, "错误", "单词本名字不能为空");
        return ;
    }
    if (nameList.contains(name + ".txt")) {
        QMessageBox::warning(this, "错误", "单词本名字重复");
        return ;
    }

    father->setCurrNotebook(name + ".txt");
    this->close();
}
