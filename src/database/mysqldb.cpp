#include "database/mysqldb.h"

MySqlDatabase::MySqlDatabase(const QString connectionName)
{
    if(QSqlDatabase::contains(connectionName)) 
    {
        db = QSqlDatabase::database(connectionName);
        if(!db.isOpenError()) return;
        QSqlDatabase::removeDatabase(connectionName);
    }
    db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
}

MySqlDatabase::~MySqlDatabase()
{
    if(db.isOpen()) {
        db.close();
    }
}

bool MySqlDatabase::connect(const QString& dbname, const QString& hostname, 
                            const QString& username, const QString& password, 
                            int port)
{
    if(db.isOpen()) db.close();

    db.setPort(port);
    db.setHostName(hostname);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(dbname);

    if(!db.open()) {
        qCritical() << "Unable to open database:" << db.lastError().text();
        return false;
    }
    return true;
}

bool MySqlDatabase::checkConnection()
{
    if (!db.isOpen()) 
    {
        qWarning() << "Database connection isn't open";
        return false;
    }
    return true;
}

bool MySqlDatabase::transaction()
{   
    if (!db.transaction()) 
    {
        qCritical() << "Failed to start transaction:" << db.lastError().text();
        return false;
    }
    return true;
}

bool MySqlDatabase::commit()
{
    if (!db.commit())
    {
        qCritical() << "Failed to commit transaction:" << db.lastError().text();
        return false;
    }
    return true;
}

bool MySqlDatabase::rollback()
{
    if (!db.rollback())
    {
        qCritical() << "Failed to rollback transaction:" << db.lastError().text();
        return false;
    }
    return true;
}
