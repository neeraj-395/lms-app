#include "managers/transacmanager.h"
#include "ui_checkoutbook.h"
#include "ui_returnbook.h"

#include "utils/qutils.h"
#include "utils/transacqueries.h"
#include "utils/transacqueries.h"
#include "database/mysqlquery.h"

TransacManager::TransacManager(TransacOp::Type type, QWidget *parent)
    : QDialog(parent)
    , checkout_ui(new Ui::CheckoutBook)
    , return_ui(new Ui::ReturnBook)
{
    switch(type)
    {
        case TransacOp::CheckoutBook:
            checkout_ui->setupUi(this);
            setCheckoutBookConnections();
            break;
        case TransacOp::ReturnBook:
            return_ui->setupUi(this);
            setReturnBookConnections();
            break;
    }
}

TransacManager::~TransacManager()
{
    delete checkout_ui;
    delete return_ui;
}


void TransacManager::handleReturnBookRequest(){}
void TransacManager::handleCheckoutBookRequest(){}

bool TransacManager::isUserExist(const uint &user_id)
{
    bool result = query_.execQuery(TransacQueries::selectUser(user_id));
    if(!result) throw std::runtime_error("Query execution failure");
    if(!query_.next()) {
        Alerts::warn(this, "User with id("+QString::number(user_id)+") is not exist!");
        return false;
    }
    return true;
}

bool TransacManager::isBookExist(const uint &book_id)
{
    bool result = query_.execQuery(TransacQueries::selectBook(book_id));
    if(!result) throw std::runtime_error("Query execution failure");
    if(!query_.next()) {
        Alerts::warn(this, "Book with id("+QString::number(book_id)+") is not exist!");
        return false;
    }
    return true;
}

bool TransacManager::isBorrowerExist(const uint &user_id, const uint &book_id)
{
    bool result = query_.execQuery(TransacQueries::selectBorr(user_id, book_id));
    if(!result) throw std::runtime_error("Query execution failure.");
    if(!query_.next()) {
        Alerts::warn(this, "User with id("+QString::number(user_id)+") has not "
                           "borrowed book with id("+QString::number(book_id)+")");
        return false;
    }
    return true;
}

const QString TransacManager::getFineAmount(QDate return_date)
{
    return QString::number(return_date.daysTo(QDate::currentDate()) * 9.99);
}


void TransacManager::verifyReturnBookRequest()
{
    try
    {
        bool user_id_ok, book_id_ok;
        const uint user_id = return_ui->userIdLineEdit->text().toUInt(&user_id_ok);
        const uint book_id = return_ui->bookIdLineEdit->text().toUInt(&book_id_ok);

        if(!user_id_ok || !book_id_ok) {
            Alerts::warn(this, "User Id or Book Id is invalid. "
                               "Please provide an integer value!");
            return;
        }

        if(isUserExist(user_id)) {
            return_ui->userNameLineEdit->setText(
                query_.record().value("name").toString()
            );
        } else return;

        if(isBookExist(book_id)) {
            return_ui->bookTitleLineEdit->setText(
                query_.record().value("title").toString()
            );
        } else return;


        if(isBorrowerExist(user_id, book_id)) {
            return_ui->checkoutDate->setDate(
                query_.record().value("checkout_date").toDate()
            );
            QDate return_date = query_.record().value("return_date").toDate();
            return_ui->returnByDate->setDate(return_date);
            return_ui->fineLineEdit->setText(getFineAmount(return_date));
        } else return;

        return_ui->resetBtn->setEnabled(true);
        return_ui->returnBtn->setEnabled(true);
        return_ui->verifyBtn->setEnabled(false);
        return_ui->bookIdLineEdit->setReadOnly(true);
        return_ui->userIdLineEdit->setReadOnly(true);

        Alerts::info(this, "Borrower successfully verfied!");
    }
    catch(std::runtime_error &e)
    {
        emit internalServiceError();
        return;
    }
}

void TransacManager::verifyCheckoutBookRequest()
{
    try
    {
        bool user_id_ok, book_id_ok;
        const uint user_id = checkout_ui->userIdLineEdit->text().toUInt(&user_id_ok);
        const uint book_id = checkout_ui->bookIdLineEdit->text().toUInt(&book_id_ok);

        if(!user_id_ok || !book_id_ok) {
            Alerts::warn(this, "User Id or Book Id is invalid. "
                               "Please provide an integer value!");
            return;
        }

        if(isUserExist(user_id)) {
            checkout_ui->userNameLineEdit->setText(
                query_.record().value("name").toString()
            );
        } else return;

        if(isBookExist(book_id)) {
            checkout_ui->bookTitleLineEdit->setText(
                query_.record().value("title").toString()
            );
            checkout_ui->bookIsbnLineEdit->setText(
                query_.record().value("isbn").toString()
            );
            checkout_ui->authorLineEdit->setText(
                query_.record().value("author").toString()
            );
        }

        checkout_ui->resetBtn->setEnabled(true);
        checkout_ui->verifyBtn->setEnabled(false);
        checkout_ui->checkoutBtn->setEnabled(true);
        checkout_ui->userIdLineEdit->setReadOnly(true);
        checkout_ui->bookIdLineEdit->setReadOnly(true);

        Alerts::info(this, "User and Book are successfully verfied!");
    }
    catch(std::runtime_error &e)
    {
        emit internalServiceError();
        return;
    }
}

void TransacManager::handleCheckoutFormResetRequest()
{
    checkout_ui->bookIdLineEdit->clear();
    checkout_ui->userIdLineEdit->clear();
    checkout_ui->userNameLineEdit->clear();
    checkout_ui->bookTitleLineEdit->clear();
    checkout_ui->bookIsbnLineEdit->clear();
    checkout_ui->authorLineEdit->clear();
    checkout_ui->verifyBtn->setEnabled(true);
    checkout_ui->resetBtn->setEnabled(false);
    checkout_ui->checkoutBtn->setEnabled(false);
    checkout_ui->userIdLineEdit->setReadOnly(false);
    checkout_ui->bookIdLineEdit->setReadOnly(false);
}

void TransacManager::handleReturnFormResetRequest()
{
    return_ui->bookIdLineEdit->clear();
    return_ui->bookTitleLineEdit->clear();
    return_ui->userIdLineEdit->clear();
    return_ui->userNameLineEdit->clear();
    return_ui->fineLineEdit->clear();
    return_ui->returnByDate->clear();
    return_ui->checkoutDate->clear();
    return_ui->verifyBtn->setEnabled(true);
    return_ui->resetBtn->setEnabled(false);
    return_ui->returnBtn->setEnabled(false);
    return_ui->userIdLineEdit->setReadOnly(false);
    return_ui->bookIdLineEdit->setReadOnly(false);
}

void TransacManager::setReturnBookConnections()
{
    connect(return_ui->verifyBtn, &QPushButton::clicked,
            this, &TransacManager::verifyReturnBookRequest);
    connect(return_ui->returnBtn, &QPushButton::clicked,
            this, &TransacManager::handleReturnBookRequest);
    connect(return_ui->resetBtn, &QPushButton::clicked,
            this, &TransacManager::handleReturnFormResetRequest);
}

void TransacManager::setCheckoutBookConnections()
{
    connect(checkout_ui->verifyBtn, &QPushButton::clicked,
            this, &TransacManager::verifyCheckoutBookRequest);
    connect(checkout_ui->checkoutBtn, &QPushButton::clicked,
            this, &TransacManager::handleCheckoutBookRequest);
    connect(checkout_ui->resetBtn, &QPushButton::clicked,
            this, &TransacManager::handleCheckoutFormResetRequest);
}
