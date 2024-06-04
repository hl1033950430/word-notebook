#include "txt_notebook_editor.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

TxtNotebookEditor::TxtNotebookEditor()
{

}


bool TxtNotebookEditor::readNotebook(const QString name, QList<WordItem> &data)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    data.clear();
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList row = line.split(this->splitCh);
        if (row.length() != 4)
            return false;
        data.append(WordItem(row[0], row[1], row[2], row[3]));
    }

    file.close();
    return true;
}


bool TxtNotebookEditor::writeNotebook(const QString name, const QList<WordItem> &data)
{
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    for (int i = 0; i < data.size(); i++)
    {
        const WordItem item = data.at(i);
        stream << item.seq << splitCh << item.english << splitCh << item.chinese << splitCh << item.mark << "\n";
    }

    file.close();
    return true;
}


QStringList TxtNotebookEditor::notebookList()
{
    QDir dir("./");
    dir.setNameFilters(QStringList("*.txt"));
    QStringList fileNames = dir.entryList(QDir::Files);
    return fileNames;
}
