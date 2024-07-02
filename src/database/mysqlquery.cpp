#include "database/mysqlquery.h"

MySqlQuery::MySqlQuery(const QString connectionName)
    :QSqlQuery(QSqlDatabase::database(connectionName)) {}

QList<QSqlRecord> MySqlQuery::getRecords()
{
    QList<QSqlRecord> records;
    while(next()) records.append(record());
    return records;
}

bool MySqlQuery::setAutoCommit(bool auto_commit)
{
    return execQuery(auto_commit ? "SET autocommit=1;" : "SET autocommit=0;");
}

bool MySqlQuery::execQuery(const QString &stmt)
{
    if(!exec(stmt)) {
        qWarning() << "Failed to execute query:" << lastError().text();
        return false;
    }
    return true;
}

bool MySqlQuery::execQuery(const QString &stmt, const QHash<QString, QVariant> &bind_values)
{
    if (!prepare(stmt)) {
        qWarning() << "Failed to prepare query:" << lastError().text();
        return false;
    }

    for (auto it = bind_values.cbegin(); it != bind_values.cend(); ++it) {
        bindValue(it.key(), it.value());
    }

    if (!exec()) {
        qWarning() << "Failed to execute query:" << lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel *MySqlQuery::getSqlQueryModel()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    // tranfer the query object ownership to query_model.
    model->setQuery(std::move(*this));

    if (model->lastError().isValid()) {
        qWarning() << "Error setting query on model:" << model->lastError().text();
        delete model;
        return nullptr;
    }
    return model;
}
