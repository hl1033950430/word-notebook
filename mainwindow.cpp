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
    connect(ui->hideChinese, &QCheckBox::stateChanged, this, &MainWindow::hideChinese);
    connect(ui->hideEnglish, &QCheckBox::stateChanged, this, &MainWindow::hideEnglish);
    connect(ui->onlyMarked, &QCheckBox::stateChanged, this, &MainWindow::hideUnmarked);
    connect(model, &QAbstractItemModel::dataChanged, this, &MainWindow::tableViewChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showNotebook(const QList<WordItem> &data)
{
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "序号（隐藏）" << "英文" << "中文" << "标记" << "删除");
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnWidth(4, 40);
    ui->tableView->setColumnWidth(3, 40);
    for (int i=0; i<data.size(); i++)
    {
        const WordItem &item = data[i];
        int newRow = model->rowCount();
        model->insertRow(newRow, QModelIndex());
        model->setData(model->index(newRow, 0), item.seq);
        model->setData(model->index(newRow, 1), item.english);
        model->setData(model->index(newRow, 2), item.chinese);
        // 单词是否被标记
        model->setData(model->index(newRow, 3), item.mark ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        model->item(newRow, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        // 删除按钮
        addDeleteButton(newRow);
    }
}

void MainWindow::updateDataFromModel()
{
    data.clear();
    data.reserve(model->rowCount());
    for (int i=0; i<model->rowCount(); i++)
    {
        // 判断单词是否被标记为删除
        if (model->index(i, 4).data().toInt())
            continue;
        // 格式转换
        QString seq = model->index(i, 0).data().toString();
        QString english = model->index(i, 1).data().toString();
        QString chinese = model->index(i, 2).data().toString();
        QString mark = model->item(i, 3)->checkState() ? "1" : "0";
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
    if (column == 3) {
        int row = index.row();
        QBrush color = model->item(row, 3)->checkState() ? QBrush(Qt::yellow) : QBrush(Qt::white);
        model->item(row, 1)->setBackground(color);
        model->item(row, 2)->setBackground(color);
    }
    updateStoreState(false);
}

void MainWindow::addWord()
{
    int newRow = model->rowCount();
    model->insertRow(newRow, QModelIndex());
    model->setData(model->index(newRow, 0), QDateTime::currentDateTime().toSecsSinceEpoch(), Qt::DisplayRole);
    model->setData(model->index(newRow, 1), "");
    model->setData(model->index(newRow, 2), "");
    model->setData(model->index(newRow, 3), Qt::Unchecked, Qt::CheckStateRole);
    model->item(newRow, 3)->setFlags((Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) & ~Qt::ItemIsEditable);
    addDeleteButton(newRow);
    // 新的单词项进入编辑状态
    ui->tableView->setFocus();
    ui->tableView->setCurrentIndex(model->index(newRow, 1));
    ui->tableView->edit(model->index(newRow, 1));
}

void MainWindow::setCurrNotebook(const QString &c)
{
    currNotebook = c;
}

void MainWindow::openNotebook()
{
    // 需要检查当前单词本的保存状态
    if (!checkStoreState())
        return ;
    // 选择已有的单词本
    NotebookList listView(this);
    listView.exec();
    // 打开
    if (currNotebook != "")
        loadNotebook(currNotebook);
    updateStoreState(true);
}

bool MainWindow::saveNotebook()
{
    if (checkAnyWordEmpty()) {
        QMessageBox::warning(this, "错误", "存在单词项为空");
        return false;
    }
    if (currNotebook == "") {
        setNewNotebookName();
        // 设置名字成功了
        if (currNotebook != "")
            notebookList.append(currNotebook);
        else
            return false;
    }
    updateDataFromModel();
    editor->writeNotebook(currNotebook, data);
    updateStoreState(true);
    return true;
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

bool MainWindow::checkStoreState()
{
    if (stored)
        return true;

    // 弹窗提醒是否保存
    // 返回 true 表示可以进行下一步
    // 若选择是，进行保存，保存成功返回 true，保存失败返回 false
    // 若选择否，不进行保存，返回 true
    // 若关闭窗口，什么都不做，不进行下一步，返回 false
    auto result = QMessageBox::question(this, "未保存", "是否保存单词本？", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if (result == QMessageBox::Yes)
        if (saveNotebook())
            return true;
        else
            return false;
    else if (result == QMessageBox::No)
        return true;
    else
        return false;
}

void MainWindow::addDeleteButton(int row)
{
    // 删除按钮
    auto buttonIndex = model->index(row, 4);
    QPushButton *button = new QPushButton("删除");
    button->setMinimumWidth(10);
    // 删除的过程：隐藏该行，将该行标记为已删除。在进行保存时，不保标记为删除的单词。
    connect(button, &QPushButton::clicked, [=](){
        ui->tableView->setRowHidden(row, true);
        model->setData(model->index(row, 4), 1);
        stored = false;
    });
    // 显示按钮。默认为未删除。
    ui->tableView->setIndexWidget(buttonIndex, button);
    model->setData(model->index(row, 4), 0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (checkStoreState())
        event->accept();
    else
        event->ignore();
}

void MainWindow::hideChinese(bool hidden)
{
    ui->tableView->setColumnHidden(1, hidden);
}

void MainWindow::hideEnglish(bool hidden)
{
    ui->tableView->setColumnHidden(0, hidden);
}

void MainWindow::hideUnmarked(bool hidden)
{
    for(int i=0; i<model->rowCount(); i++)
        if (!model->item(i, 2)->checkState())
            ui->tableView->setRowHidden(i, hidden);
}

const QStringList& MainWindow::getNotebookList()
{
    return this->notebookList;
}
