#include <QtTest>
#include <mysqldb.h>

class TestMySqlDatabase : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testOpen();
    void testExecQuery();
    void testConfigFileExists();
    void testConfigFileKeys();
};

void TestMySqlDatabase::initTestCase() {}

void TestMySqlDatabase::cleanupTestCase() {}

void TestMySqlDatabase::testOpen() {
    MySqlDatabase db;
    QVERIFY(db.open());  // Ensure the database opens successfully.
}

void TestMySqlDatabase::testExecQuery() {
    MySqlDatabase db;

    QVERIFY(db.execQuery("SELECT * FROM users WHERE id = :id", {{":id", 1}}));
}

void TestMySqlDatabase::testConfigFileExists() {
    QFile configFile(":/db.config.ini");
    QVERIFY(configFile.exists());  // Ensure the config file exists.
}

void TestMySqlDatabase::testConfigFileKeys() {
    QSettings config(":/db.config.ini", QSettings::IniFormat);

    QVERIFY(config.contains("DB_PORT"));
    QVERIFY(config.contains("DB_HOSTNAME"));
    QVERIFY(config.contains("DB_USERNAME"));
    QVERIFY(config.contains("DB_PASSWORD"));
    QVERIFY(config.contains("DB_NAME"));
}


QTEST_MAIN(TestMySqlDatabase)
#include "testmysqldb.moc"
