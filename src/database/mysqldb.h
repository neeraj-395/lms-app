#ifndef MYSQLDB_H
#define MYSQLDB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QDebug>

class MySqlDatabase {

public:
    explicit MySqlDatabase(const QString connectionName = QLatin1String(
        QSqlDatabase::defaultConnection
    ));

    ~MySqlDatabase();

    bool connect(const QString& dbname, const QString& hostname = "localhost", 
                 const QString& username = "root", const QString& password = "", 
                 int port = 3306);

    bool commit();
    bool rollback();
    bool transaction();
    bool createDatabase();
    bool checkConnection();

private:
    QSqlDatabase db;
    MySqlDatabase(const MySqlDatabase&) = delete;
    MySqlDatabase& operator=(const MySqlDatabase&) = delete;
};

#endif // MYSQLDB_H
