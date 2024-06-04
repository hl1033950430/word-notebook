#include "notebook_list_window.h"
#include "ui_notebook_list.h"

NotebookList::NotebookList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotebookList)
{
    ui->setupUi(this);

    // init list view
    MainWindow *father = (MainWindow*) parent;
    const QStringList &notebookNames = father->getNotebookList();
    ui->listView->setModel(new QStringListModel(notebookNames));

    // double click
    connect(ui->listView, &QListView::doubleClicked, this, &NotebookList::selectNotebook);
}

NotebookList::~NotebookList()
{
    delete ui;
}

void NotebookList::selectNotebook(const QModelIndex &index)
{
    MainWindow *father = (MainWindow*)parent();
    father->setCurrNotebook(index.data().toString());
    this->close();
}
