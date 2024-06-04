#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QDateTime>
#include <QMessageBox>
#include <memory>
#include "worditem.h"
#include "notebook_editor.h"
#include "txt_notebook_editor.h"
#include "notebook_list_window.h"
#include "input_name_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadNotebook(const QString &notebookName);

    void setCurrNotebook(const QString &c);
    const QStringList& getNotebookList();

private slots:
    void openNotebook();
    void saveNotebook();
    void tableViewChanged(const QModelIndex &index);
    void addWord();

private:
    Ui::MainWindow *ui;
    // 用于单词本的读写
    std::shared_ptr<NotebookEditor> editor;
    // 当前单词本，单词本列表
    QString currNotebook;
    QStringList notebookList;
    // 单词本的数据，data 用于读写单词本，model 用于界面显示
    QList<WordItem> data;
    QStandardItemModel *model;
    // 是否保存
    bool stored;

    void showNotebook(const QList<WordItem> &data);
    // 将 model 的数据更新到 data, model 的数据可能在界面上被用户编辑
    void updateDataFromModel();
    // 检查 model 中是否有单词项为空
    bool checkAnyWordEmpty();
    // 打开一个窗口设置单词本名字
    void setNewNotebookName();
    // 更新是否存储的状态
    void updateStoreState(bool stored);
};
#endif // MAINWINDOW_H
