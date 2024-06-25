#include "database/mysqlquery.h"

MySqlQuery::MySqlQuery() : QSqlQuery(QSqlDatabase::database()) {}

QList<QSqlRecord> MySqlQuery::getRecords()
{
    QList<QSqlRecord> records;
    while(next()) records.append(record());
    return records;
}

bool MySqlQuery::execQuery(const QString stmt)
{
    if(exec(stmt)) {
        qWarning() << "Failed to execute query:" << lastError();
        return false;
    }
    return true;
}

bool MySqlQuery::execQuery(const QString stmt, const QHash<QString, QVariant> bind_values) 
{
    if (!prepare(stmt)) {
        qWarning() << "Failed to prepare query:" << lastError();
        return false;
    }

    for (auto it = bind_values.cbegin(); it != bind_values.cend(); ++it) {
        bindValue(it.key(), it.value());
    }

    if (!exec()) {
        qWarning() << "Failed to execute query:" << lastError();
        return false;
    }

    return true;
}