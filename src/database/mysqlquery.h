#ifndef MYSQLQUERY_H
#define MYSQLQUERY_H

#include <QDebug>
#include <QString>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>

class MySqlQuery : public QSqlQuery {

public:
    explicit MySqlQuery(const QString connectionName = QLatin1String(
        QSqlDatabase::defaultConnection
    ));

    QList<QSqlRecord> getRecords();
    bool setAutoCommit(bool auto_commit);
    bool execQuery(const QString &stmt);
    bool execQuery(const QString &stmt, const QHash<QString, QVariant> &bind_values);
    QSqlQueryModel *getSqlQueryModel();
};

#endif // MYSQLDB_H
