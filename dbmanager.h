#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DBManager
{
public:
    static void initialize();
    static QSqlDatabase getDB();
};

#endif // DBMANAGER_H
