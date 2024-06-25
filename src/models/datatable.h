#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QSqlRecord>

class DataTable : public QWidget
{
    Q_OBJECT
public:
    explicit DataTable(const QString title, QWidget *parent = nullptr);

    void populateTable(const QList<QSqlRecord> data, const char* headers[]);

private:
    QTableWidget *tableWidget;
};

#endif // DATATABLE_H
