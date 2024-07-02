#include "managers/transacmanager.h"
#include "ui_checkoutbook.h"
#include "ui_returnbook.h"

#include "utils/qutils.h"
#include "utils/transacqueries.h"

TransacManager::TransacManager(TransacOp::Type type, QWidget *parent)
    : QDialog(parent)
    , checkout_ui(nullptr)
    , return_ui(nullptr)
{
    switch(type)
    {
        case TransacOp::CheckoutBook:
            checkout_ui = new Ui::CheckoutBook;
            checkout_ui->setupUi(this);
            setCheckoutBookConnections();
            checkout_ui->returnOnDate->
            setMinimumDate(QDate::currentDate());
            break;
        case TransacOp::ReturnBook:
            return_ui = new Ui::ReturnBook;
            return_ui->setupUi(this);
            setReturnBookConnections();
            break;
    }
}

TransacManager::~TransacManager()
{
    if(checkout_ui) delete checkout_ui;
    if(return_ui) delete return_ui;
}

namespace Verify
{
    template <typename transac_ui>
    static void setVerificationState(bool enabled, transac_ui* ui) {
        ui->verifyBtn->setEnabled(!enabled);
        ui->resetBtn->setEnabled(enabled);
        ui->proceedBtn->setEnabled(enabled);
        ui->userIdSpinBox->setReadOnly(enabled);
        ui->bookIdSpinBox->setReadOnly(enabled);
    }
}

bool TransacManager::isUserExist(const uint &user_id, MySqlQuery &query)
{
    bool result = query.execQuery(TransacQueries::selectUser(user_id));
    if(!result) { emit internalServiceError(); return false; }
    if(!query.next()) {
        Alerts::warn(this, "User with id: "+QString::number(user_id)+" is not exist!");
        return false;
    }
    return true;
}

bool TransacManager::isBookExist(const uint &book_id, MySqlQuery &query)
{
    bool result = query.execQuery(TransacQueries::selectBook(book_id));
    if(!result) { emit internalServiceError(); return false; }
    if(!query.next()) {
        Alerts::warn(this, "Book with id: "+QString::number(book_id)+" is not exist!");
        return false;
    }
    return true;
}

const float TransacManager::getFineAmount(QDate return_date)
{
    return return_date.daysTo(QDate::currentDate()) * 9.99f; // per extra day charge of amount: 9.99₹
}

void TransacManager::handleReturnBookRequest()
{
    bool result;
    const uint book_id = return_ui->bookIdSpinBox->value();
    const float fine_amount = return_ui->fineSpinBox->value();

    MySqlDatabase db; MySqlQuery query;

    if(!query.setAutoCommit(false) || !db.transaction()) {
        emit internalServiceError();
        Verify::setVerificationState(false, return_ui);
        return;
    }

    result = query.execQuery(TransacQueries::updateReturned(borrower_id_));
    if(!result || !query.numRowsAffected()) {
        handleTransacFailure(db, query);
        return;
    };

    result = query.execQuery(TransacQueries::incBookQuantity(book_id));
    if(!result || !query.numRowsAffected()) {
        handleTransacFailure(db, query);
        return;
    };

    result = query.execQuery(TransacQueries::addfine(borrower_id_, fine_amount));
    if(!result || !query.numRowsAffected()) {
        handleTransacFailure(db, query);
        return;
    };

    if(!db.commit()) {
        handleTransacFailure(db, query);
        return;
    }

    query.setAutoCommit(true);
    return_ui->resetBtn->click();
    Alerts::info(this, "Book return successful! Thank you for returning the book.");
}

void TransacManager::handleCheckoutBookRequest()
{
    bool result;
    const uint user_id = checkout_ui->userIdSpinBox->value();
    const uint book_id = checkout_ui->bookIdSpinBox->value();
    const QDate return_date = checkout_ui->returnOnDate->date();

    if(return_date == QDate::currentDate()) {
        Alerts::warn(this, "Return date cannot be the current date. "
                           "Please set the return date according to "
                           "the specified borrowing duration.");
        return;
    }

    MySqlDatabase db; MySqlQuery query;

    if(!query.setAutoCommit(false) || !db.transaction()) {
        emit internalServiceError();
        Verify::setVerificationState(false, checkout_ui);
        return;
    }

    result = query.execQuery(TransacQueries::addBorrower(), {
        {":user_id", user_id},
        {":book_id", book_id},
        {":return_date", return_date}
    });

    if(!result || !query.numRowsAffected()) {
        handleTransacFailure(db, query);
        return;
    }

    result = query.execQuery(TransacQueries::decBookQuantity(book_id));
    if(!result || !query.numRowsAffected()) {
        handleTransacFailure(db, query);
        return;
    }

    if(!db.commit()) {
        handleTransacFailure(db, query);
        return;
    }

    query.setAutoCommit(true);
    checkout_ui->resetBtn->click();
    Alerts::info(this, "Checkout successful! You have successfully checked out the book.");
}


void TransacManager::verifyReturnBookRequest()
{
    const uint user_id = return_ui->userIdSpinBox->value();
    const uint book_id = return_ui->bookIdSpinBox->value();

    MySqlQuery query;

    if(!isUserExist(user_id, query)) return;
    return_ui->userNameLineEdit->setText(query.record().value("name").toString());

    if(!isBookExist(book_id, query)) return;
    return_ui->bookTitleLineEdit->setText(query.record().value("title").toString());

    bool result = query.execQuery(TransacQueries::selectBorrower(user_id, book_id));
    if(!result) { emit internalServiceError(); return; }
    if(!query.next()) { alertNotBorrowed(user_id, book_id); return;}

    borrower_id_ = query.record().value("id").toUInt();
    return_ui->checkoutDate->setDate(query.record().value("checkout_date").toDate());
    QDate return_date = query.record().value("return_date").toDate();
    return_ui->returnOnDate->setDate(return_date);
    return_ui->fineSpinBox->setValue(getFineAmount(return_date));

    Verify::setVerificationState(true, return_ui);
    Alerts::info(this, "Verification successful! now you can proceed to return book");
}

void TransacManager::verifyCheckoutBookRequest()
{
    const uint user_id = checkout_ui->userIdSpinBox->value();
    const uint book_id = checkout_ui->bookIdSpinBox->value();

    MySqlQuery query;

    bool result = query.execQuery(TransacQueries::selectBorrower(user_id, book_id));
    if(!result) { emit internalServiceError(); return; }
    if(query.next()) { alertNotReturned(user_id, book_id); return;}

    if(!isUserExist(user_id, query)) return;
    checkout_ui->userNameLineEdit->setText(query.record().value("name").toString());

    if(!isBookExist(book_id, query)) return;
    checkout_ui->bookTitleLineEdit->setText(query.record().value("title").toString());
    checkout_ui->bookIsbnLineEdit->setText(query.record().value("isbn").toString());
    checkout_ui->authorLineEdit->setText(query.record().value("author").toString());

    Verify::setVerificationState(true, checkout_ui);
    Alerts::info(this, "Verification is successful! now you can proceed to checkout.");
}

void TransacManager::handleTransacFailure(MySqlDatabase &db, MySqlQuery &query)
{
    if(!db.rollback()) {
        Alerts::critical(this, "Transaction rollback failed. "
                               "Immediate admin intervention required.");
    } else {
        emit internalServiceError();
    }
    query.setAutoCommit(true);
    if(checkout_ui) checkout_ui->resetBtn->click();
    if(return_ui) return_ui->resetBtn->click();
}

void TransacManager::alertNotBorrowed(const uint &user_id, const uint &book_id)
{
    Alerts::warn(this, "User with Id: " + QString::number(user_id) + " hasn't borrowed "
    "Book with Id: " + QString::number(book_id) + ". Please check details carefully!");
}

void TransacManager::alertNotReturned(const uint &user_id, const uint &book_id)
{
    Alerts::warn(this, "User with Id: " + QString::number(user_id) + " hasn't returned "
    "Book with Id: " + QString::number(book_id) + " yet. Borrowing the same book is not permitted!");
}


void TransacManager::handleCheckoutFormResetRequest()
{
    checkout_ui->bookIdSpinBox->clear();
    checkout_ui->userIdSpinBox->clear();
    checkout_ui->userNameLineEdit->clear();
    checkout_ui->bookTitleLineEdit->clear();
    checkout_ui->bookIsbnLineEdit->clear();
    checkout_ui->authorLineEdit->clear();
    checkout_ui->returnOnDate->setDate(QDate::currentDate());
    Verify::setVerificationState(false, checkout_ui);
}

void TransacManager::handleReturnFormResetRequest()
{
    return_ui->bookIdSpinBox->clear();
    return_ui->bookTitleLineEdit->clear();
    return_ui->userIdSpinBox->clear();
    return_ui->userNameLineEdit->clear();
    return_ui->fineSpinBox->clear();
    return_ui->returnOnDate->setDate(QDate::currentDate());
    return_ui->checkoutDate->setDate(QDate::currentDate());
    Verify::setVerificationState(false, return_ui);
}

void TransacManager::setReturnBookConnections()
{
    connect(return_ui->verifyBtn, &QPushButton::clicked,
            this, &TransacManager::verifyReturnBookRequest);
    connect(return_ui->proceedBtn, &QPushButton::clicked,
            this, &TransacManager::handleReturnBookRequest);
    connect(return_ui->resetBtn, &QPushButton::clicked,
            this, &TransacManager::handleReturnFormResetRequest);
}

void TransacManager::setCheckoutBookConnections()
{
    connect(checkout_ui->verifyBtn, &QPushButton::clicked,
            this, &TransacManager::verifyCheckoutBookRequest);
    connect(checkout_ui->proceedBtn, &QPushButton::clicked,
            this, &TransacManager::handleCheckoutBookRequest);
    connect(checkout_ui->resetBtn, &QPushButton::clicked,
            this, &TransacManager::handleCheckoutFormResetRequest);
}
