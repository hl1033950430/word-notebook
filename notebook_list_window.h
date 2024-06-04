#ifndef NOTEBOOK_LIST_H
#define NOTEBOOK_LIST_H

#include <QDialog>
#include <QDebug>
#include <QStringListModel>
#include "mainwindow.h"


namespace Ui {
class NotebookList;
}

class NotebookList : public QDialog
{
    Q_OBJECT

public:
    explicit NotebookList(QWidget *parent = nullptr);
    ~NotebookList();

private slots:
    void selectNotebook(const QModelIndex &index);

private:
    Ui::NotebookList *ui;
};

#endif // NOTEBOOK_LIST_H
