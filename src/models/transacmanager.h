#ifndef TRANSACMANAGER_H
#define TRANSACMANAGER_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui { class CheckoutBook; class ReturnBook; }

namespace TransacOp{ enum Type{ CheckoutBook, ReturnBook }; }

class TransacManager : public QDialog
{
    Q_OBJECT

public:
    explicit TransacManager(TransacOp::Type type, QWidget *parent = nullptr);
    ~TransacManager();

private:
    Ui::CheckoutBook *checkoutUi;
    //Ui::ReturnBook *returnUi;
    TransacOp::Type operation;
    /*void setupCheckoutConnect(); // to setup checkout book ui connections
    void setupReturnConnect(); // to setup return book ui connections
    void verifyBook();
    void returnBook();
    void checkoutBook();
    void calculateFine();
    QList<QSqlRecord> execQuery(const QString stmt, bool fetchRecords,
                                const QHash<QString, QVariant> bindV);*/
};

#endif // TRANSACMANAGER_H
