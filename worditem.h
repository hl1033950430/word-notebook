#ifndef WORDITEM_H
#define WORDITEM_H

#include<QString>

class WordItem
{
public:
    WordItem();
    WordItem(QString s, QString e, QString c, QString m);

    // 序号（时间戳）、英文、中文、是否标记
    long long seq;
    QString english;
    QString chinese;
    bool mark;

};

#endif // WORDITEM_H
