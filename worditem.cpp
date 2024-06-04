#include "worditem.h"

WordItem::WordItem()
{

}

WordItem::WordItem(QString s, QString e, QString c, QString m)
    :seq(s.toLongLong()), english(e), chinese(c), mark(m.toInt())
{
}
