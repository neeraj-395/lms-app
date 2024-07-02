#ifndef TRANSACMANAGER_H
#define TRANSACMANAGER_H

#include <QDialog>
#include <QLineEdit>

#include "database/mysqldb.h"
#include "database/mysqlquery.h"

namespace TransacOp {
    enum Type {
        CheckoutBook,
        ReturnBook
    };
}

namespace Ui {
    class CheckoutBook;
    class ReturnBook;
}

class TransacManager : public QDialog
{
    Q_OBJECT

public:
    explicit TransacManager(TransacOp::Type type, QWidget *parent = nullptr);
    ~TransacManager();

private slots:
    void verifyReturnBookRequest();
    void verifyCheckoutBookRequest();
    void handleReturnBookRequest();
    void handleCheckoutBookRequest();
    void handleReturnFormResetRequest();
    void handleCheckoutFormResetRequest();

signals:
    void internalServiceError();

private:
    uint             borrower_id_;
    Ui::ReturnBook   *return_ui;
    Ui::CheckoutBook *checkout_ui;

private:
    void setReturnBookConnections();
    void setCheckoutBookConnections();
    const float getFineAmount(const QDate return_date);
    bool isUserExist(const uint &user_id, MySqlQuery &query);
    bool isBookExist(const uint &book_id, MySqlQuery &query);
    void alertNotReturned(const uint &user_id, const uint &book_id);
    void alertNotBorrowed(const uint &user_id, const uint &book_id);
    void handleTransacFailure(MySqlDatabase &db, MySqlQuery &query);
    bool isBorrowerExist(const uint &user_id, const uint &book_id, MySqlQuery &query);
};

#endif // TRANSACMANAGER_H
