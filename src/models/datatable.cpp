#include <QVBoxLayout>
#include <QHeaderView>

#include "models/datatable.h"

DataTable::DataTable(const QString title, QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle(title);
    tableWidget = new QTableWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    setLayout(layout);
    setAttribute(Qt::WA_DeleteOnClose);
}

void DataTable::populateTable(const QList<QSqlRecord> data, const char* headers[])
{
    int rowCount = data.size();
    int colCount = data[0].count();

    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(colCount);

    for(int i=0; i < colCount; ++i) {
        tableWidget->model()->setHeaderData(i, Qt::Horizontal, headers[i]);
    }

    for(int row=0; row < rowCount; ++row){
        for(int col=0; col < colCount; ++col){
            QTableWidgetItem *item = new QTableWidgetItem(data[row].value(col).toString());
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, col, item);
        }
    }

    tableWidget->resizeColumnsToContents();
    tableWidget->resizeRowsToContents();

    int totalWidth = 0;
    for (int col = 0; col < colCount; ++col) {
        totalWidth += tableWidget->columnWidth(col);
    }
    int totalHeight = tableWidget->horizontalHeader()->height()
                    + tableWidget->verticalHeader()->length();
    this->resize(totalWidth + tableWidget->verticalHeader()->width()
                            + 24, totalHeight);

}
