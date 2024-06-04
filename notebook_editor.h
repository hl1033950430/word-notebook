#ifndef WORDEDITOR_H
#define WORDEDITOR_H

#include <QString>
#include "worditem.h"

class NotebookEditor
{
public:
    NotebookEditor();
    // 读写单词本、展示单词本列表
    virtual bool readNotebook(const QString name, QList<WordItem> &data) = 0;
    virtual bool writeNotebook(const QString name, const QList<WordItem> &data) = 0;
    virtual QStringList notebookList() = 0;
};

#endif // WORDEDITOR_H
