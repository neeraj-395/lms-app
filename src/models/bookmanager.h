#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include <QList>
#include <QDialog>
#include <QLineEdit>
#include <QSqlRecord>

#include "database/mysqlquery.h"

namespace Ui { class BookManager; }

namespace BookOp
{ 
    enum Type{ AddBook, RemoveBook, SearchBook, EditBook };
}

class BookManager : public QDialog
{
    Q_OBJECT

public:
    explicit BookManager(QWidget *parent = nullptr);
    ~BookManager();

    void initManager(const QString title, BookOp::Type type);

private slots:
    void addBook();
    void editBook();
    void searchBook();
    void removeBook();
    void loadBookData();

protected:
    using QWidget::show;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::BookManager   *ui;
    BookOp::Type      operation;
    QList<QLineEdit*> lineEditList;

private:
    void setupConnection();
    void setupUiComponents();
};

#endif // BOOKMANAGER_H
