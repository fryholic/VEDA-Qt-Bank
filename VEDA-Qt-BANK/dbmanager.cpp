#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

void DBManager::initialize() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    db.setDatabaseName(path + "/bank.db");

    if (!db.open()) {
        qCritical() << "Database connection error:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS accounts ("
               "accountNumber TEXT PRIMARY KEY,"
               "accountName TEXT,"
               "balance REAL)");

    query.exec("CREATE TABLE IF NOT EXISTS transactions ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "accountNumber TEXT,"
               "date TEXT,"
               "type TEXT,"
               "amount REAL,"
               "description TEXT)");
}
