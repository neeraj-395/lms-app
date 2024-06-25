#include <QMessageBox>

#include "models/transacmanager.h"
#include "ui_checkoutbook.h"
#include "ui_returnbook.h"

TransacManager::TransacManager(TransacOp::Type type, QWidget *parent)
    : QDialog(parent)
    , checkoutUi(new Ui::CheckoutBook)
    //, returnUi(new Ui::ReturnBook)
    , operation(type)
{
    checkoutUi->setupUi(this);
}

TransacManager::~TransacManager()
{
    delete checkoutUi;
}

/*void TransacManager::returnBook() {
    QString stmt = "SELECT id FROM borrowers WHERE user_id = :user_id "
                   "AND book_id = :book_id AND return_status = FALSE";

    QString userId = returnUi->userIdLE->text();
    QString bookId = returnUi->bookIdLE->text();

    QList<QSqlRecord> data = execQuery(stmt, true, {
        {"user_id", userId}, 
        {"book_id", bookId}
    });

    if(data.isEmpty()){
        QMessageBox::information(this, "", "Borrower where not found!");
        return;
    }

    stmt = "UPDATE borrowers SET return_status = TRUE WHERE borrower_id = :bor_id";

    execQuery(stmt, false, {{"bor_id", data.at(0).value("id").toInt()}});
}

void TransacManager::checkoutBook() {
    QString stmt = "INSERT INTO borrowers(user_id, book_id, return_by) "
                   "VALUES(:user_id, :book_id, :return_by)";
    QString userId = checkoutUi->userIdLE->text();
    QString bookId = checkoutUi->bookIdLE->text();
    QDate returnBy = checkoutUi->returnByDE->date();

    execQuery(stmt, false, {
        {"user_id", userId}, 
        {"book_id", bookId}, 
        {"return_by", returnBy}
    });
}

void TransacManager::verifyBook() {
    QString stmt = "SELECT *FROM books WHERE id = :book_id LIMIT 1";

    QString bookId =  checkoutUi->bookIdLE->text();

    QList<QSqlRecord> data  = execQuery(stmt, true, {{"book_id", bookId}});

    if(!data.isEmpty()){
        QMessageBox::information(this, "", "Book where not found");
        return;
    }

    if(!data.at(0).value("available").toBool()) {
        QMessageBox::information(this, "", "Sorry book is currently not available");
        return;
    }

    checkoutUi->bookTitleLE->setText(data.at(0).value("title").toString());
    checkoutUi->bookIsbnLB->setText(data.at(0).value("isbn").toString());
    checkoutUi->checkoutBtn->setEnabled(true);
    //checkoutUi->bookAuthor->setText(data.at(0).value("isbn").toString());
}

void TransacManager::calculateFine() {}

void TransacManager::setupReturnConnect() {
    connect(returnUi->verifyBtn, &QPushButton::clicked, this, &TransacManager::verifyBook);
    connect(returnUi->fineBtn, &QPushButton::clicked, this, &TransacManager::returnBook);
    connect(returnUi->returnBtn, &QPushButton::clicked, this, &TransacManager::returnBook);
}

void TransacManager::setupCheckoutConnect() {
    connect(checkoutUi->verifyBtn, &QPushButton::clicked, this, &TransacManager::verifyBook);
    connect(checkoutUi->checkoutBtn, &QPushButton::clicked, this, &TransacManager::returnBook);
}

QList<QSqlRecord> TransacManager::execQuery(const QString stmt, bool fetchRecords,
                                         const QHash<QString, QVariant> bindV) {
    try{
        MySqlDatabase db;

        db.prepare(stmt, bindV);

        int numRowsAffected = db.exec();

        if(fetchRecords) return db.getRecords();

        if (!numRowsAffected) {
            QMessageBox::warning(this, "Operation Failed", "The operation did not succeed.");
        } else {
            QMessageBox::information(this, "Operation Successful", "The operation was successful.");
        }
        return QList<QSqlRecord>();

    } catch(MySqlException &e){
        QMessageBox::critical(this, "Database Error", e.what());
        return QList<QSqlRecord>();
    }
}
*/