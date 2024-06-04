#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , editor(std::make_shared<TxtNotebookEditor>())
    , currNotebook("")
    , model(new QStandardItemModel())
    , stored(true)
{
    ui->setupUi(this);
    notebookList = editor->notebookList();

    ui->tableView->setModel(model);
    showNotebook({});

    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openNotebook);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addWord);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveNotebook);
    connect(model, &QAbstractItemModel::dataChanged, this, &MainWindow::tableViewChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showNotebook(const QList<WordItem> &data)
{
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "英文" << "中文" << "标记");
    for (int i=0; i<data.size(); i++)
    {
        const WordItem &item = data[i];
        int newRow = model->rowCount();
        model->insertRow(newRow, QModelIndex());
        model->setData(model->index(newRow, 0), item.english);
        model->setData(model->index(newRow, 1), item.chinese);
        model->setData(model->index(newRow, 2), item.mark ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        model->setData(model->index(newRow, 3), item.seq, ~Qt::DisplayRole);
        model->item(newRow, 2)->setFlags((Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) & ~Qt::ItemIsEditable);
    }
}

void MainWindow::updateDataFromModel()
{
    data.clear();
    data.reserve(model->rowCount());
    for (int i=0; i<model->rowCount(); i++)
    {
        QString seq = model->index(i, 3).data().toString();
        QString english = model->index(i, 0).data().toString();
        QString chinese = model->index(i, 1).data().toString();
        QString mark = model->item(i, 2)->checkState() ? "1" : "0";
        data.push_back(WordItem(seq, english, chinese, mark));
    }
}

bool MainWindow::checkAnyWordEmpty()
{
    for (int i=0; i<model->rowCount(); i++)
        if (model->index(i, 0).data().toString() == "" && model->index(i, 1).data().toString() == "")
            return true;
    return false;
}

void MainWindow::loadNotebook(const QString &notebookName)
{
    // 读取单词本内容
    editor->readNotebook(notebookName, data);
    currNotebook = notebookName;

    // 进行显示
    setWindowTitle(notebookName);
    showNotebook(data);
}

void MainWindow::tableViewChanged(const QModelIndex &index)
{
    int column = index.column();
    // 标记的单词项高亮显示
    if (column == 2) {
        int row = index.row();
        QBrush color = model->item(row, 2)->checkState() ? QBrush(Qt::yellow) : QBrush(Qt::white);
        model->item(row, 0)->setBackground(color);
        model->item(row, 1)->setBackground(color);
    }
    updateStoreState(false);
}

void MainWindow::addWord()
{
    int newRow = model->rowCount();
    model->insertRow(newRow, QModelIndex());
    model->setData(model->index(newRow, 0), "");
    model->setData(model->index(newRow, 1), "");
    model->setData(model->index(newRow, 2), Qt::Unchecked, Qt::CheckStateRole);
    model->setData(model->index(newRow, 3), QDateTime::currentDateTime().toSecsSinceEpoch(), ~Qt::DisplayRole);
    model->item(newRow, 2)->setFlags((Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) & ~Qt::ItemIsEditable);
    // 新的单词项进入编辑状态
    ui->tableView->setFocus();
    ui->tableView->setCurrentIndex(model->index(newRow, 0));
    ui->tableView->edit(model->index(newRow, 0));
}

void MainWindow::setCurrNotebook(const QString &c)
{
    currNotebook = c;
}

void MainWindow::openNotebook()
{
    // 选择已有的单词本
    NotebookList listView(this);
    listView.exec();
    // 打开
    if (currNotebook != "")
        loadNotebook(currNotebook);
    updateStoreState(true);
}

void MainWindow::saveNotebook()
{
    if (checkAnyWordEmpty()) {
        QMessageBox::warning(this, "错误", "存在单词项为空");
        return ;
    }
    if (currNotebook == "") {
        setNewNotebookName();
        return ;
    }
    updateDataFromModel();
    editor->writeNotebook(currNotebook, data);
    updateStoreState(true);
}

void MainWindow::setNewNotebookName()
{
    InputNameWindow inputNameWindow(this);
    inputNameWindow.exec();
    if (currNotebook != "") {
        notebookList.append(currNotebook);
        saveNotebook();
    }
}

void MainWindow::updateStoreState(bool stored)
{
    this->stored = stored;
    if (stored)
        this->setWindowTitle(currNotebook);
    else
        this->setWindowTitle(currNotebook + " *");
}

const QStringList& MainWindow::getNotebookList()
{
    return this->notebookList;
}
