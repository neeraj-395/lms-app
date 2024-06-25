#ifndef MYSQLQUERY_H
#define MYSQLQUERY_H

#include <QDebug>
#include <QString>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class MySqlQuery : public QSqlQuery {

public:
    MySqlQuery();
    QList<QSqlRecord> getRecords();
    bool execQuery(const QString stmt);
    bool execQuery(const QString stmt, const QHash<QString, QVariant> bind_values);
};

#endif // MYSQLDB_H
