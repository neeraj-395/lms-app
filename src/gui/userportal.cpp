#include <QMessageBox>

#include "gui/userportal.h"
#include "ui_userportal.h"

#include "models/datatable.h"
#include "database/mysqlquery.h"

namespace TableHeaders {
    static const char* BookBorrowed[] = {
        "Title", "Author", "ISBN", "Checkout Date", "Return by", "Returned?"
    };
    static const char* FineDetails[] = {
        "Fine Id", "Title", "ISBN", "Fine Amount(Rs)", "Paid?"
    };
}

namespace UserQueries {
    static const char* BookBorrowed = R"(
        SELECT b.title, b.author, b.isbn, bor.checkout_date, 
        bor.return_by, bor.return_status FROM borrowers bor 
        JOIN books b ON bor.book_id = b.id JOIN users u 
        ON bor.user_id = u.id WHERE u.id = :user_id;
    )";

    static const char* FineDetails = R"(
        SELECT f.id, b.title, b.isbn, f.fine_amount, f.pay_status 
        FROM fines f JOIN borrowers bor ON f.borrower_id = bor.id 
        JOIN books b ON bor.book_id = b.id WHERE bor.user_id = :user_id;
    )";
}

UserPortal::UserPortal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserPortal)
    , book_ui(new BookManager(this))
{
    ui->setupUi(this);
    setupConnections();
}

UserPortal::~UserPortal()
{
    delete ui;
}

void UserPortal::bookSearch()
{
    book_ui->initManager("Search for Books", BookOp::SearchBook);
}


void UserPortal::viewBookBorrowed()
{
    MySqlQuery query;

    bool result = query.execQuery(UserQueries::BookBorrowed, {
        {":user_id", userId}
    });

    if(!result) {
        QMessageBox::critical(this, "Operation failed", "Database error occured");
        return;
    }

    QList<QSqlRecord> data = query.getRecords();

    if(data.isEmpty()){
        QMessageBox::information(this, "Not Found", "No Book was Borrowed yet!");
        return;
    }

    DataTable *bookBorrowedTable = new DataTable("Book Borrowed Data");
    bookBorrowedTable->populateTable(data, TableHeaders::BookBorrowed);
    bookBorrowedTable->show();
}


void UserPortal:: viewFineDetails()
{
    MySqlQuery query;

    bool result = query.execQuery(UserQueries::FineDetails, {
        {":user_id", userId}
    });

    if(!result) {
        QMessageBox::critical(this, "Operation failed", "Database error occured");
        return;
    }

    QList<QSqlRecord> data = query.getRecords();

    if(data.isEmpty()){
        QMessageBox::information(this, "Not Found", "No due fine is pending!");
    }

    DataTable *fineDetailsTable = new DataTable("Fine Details Data");
    fineDetailsTable->populateTable(data , TableHeaders::FineDetails);
    fineDetailsTable->show();
}

void UserPortal::setUser(quint8 id, QString name){
    this->userId = id;
    this->userName = name;
    ui->logoutBox->setTitle(name);
}

void UserPortal::setupConnections()
{
    connect(ui->logoutBtn, &QPushButton::clicked, this, &UserPortal::backToMain);

    connect(ui->searchBook, &QPushButton::clicked, this, &UserPortal::bookSearch);

    connect(ui->viewFineDetails, &QPushButton::clicked, this, &UserPortal::viewFineDetails);

    connect(ui->viewBookBorrowed, &QPushButton::clicked, this, &UserPortal::viewBookBorrowed);
}
