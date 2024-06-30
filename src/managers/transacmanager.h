#ifndef TRANSACMANAGER_H
#define TRANSACMANAGER_H

#include <QDialog>

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
    QString           borr_id_;
    MySqlQuery        query_;
    Ui::ReturnBook   *return_ui;
    Ui::CheckoutBook *checkout_ui;

private:
    void setReturnBookConnections();
    void setCheckoutBookConnections();
    bool isUserExist(const uint &user_id);
    bool isBookExist(const uint &book_id);
    const QString getFineAmount(const QDate return_date);
    bool isBorrowerExist(const uint &user_id, const uint &book_id);
};

#endif // TRANSACMANAGER_H
