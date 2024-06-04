#ifndef TXTWORDEDITOR_H
#define TXTWORDEDITOR_H

#include "notebook_editor.h"

/**
 * @brief The TxtWordEditor class
 * 从文本文件中进行单词本的读写
 */

class TxtNotebookEditor : public NotebookEditor
{
public:
    TxtNotebookEditor();
    bool readNotebook(const QString name, QList<WordItem> &data) override;
    bool writeNotebook(const QString name, const QList<WordItem> &data) override;
    QStringList notebookList() override;
private:
    char splitCh = '|';
};

#endif // TXTWORDEDITOR_H
