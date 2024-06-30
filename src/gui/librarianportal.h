#ifndef LIBRARIANPORTAL_H
#define LIBRARIANPORTAL_H

#include <QWidget>

#include "managers/bookmanager.h"
#include "managers/transacmanager.h"

namespace Ui { class LibrarianPortal; }

class LibrarianPortal : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianPortal(QWidget *parent = nullptr);
    ~LibrarianPortal();

    void setLibrarian(const uint id, const QString &name);

signals:
    void logoutRequestToMain();

private slots:
    void handleLogoutRequest();
    void handleInternalServiceError();

private:
    Ui::LibrarianPortal *ui;
    BookManager         *book_wd;
    TransacManager      *checkout_wd; // will generate mysql opening error
    TransacManager      *return_wd; // will generate mysql opening error
    uint                librarian_id;
    QString             librarian_name;

private:
    void setupConnection();
};

#endif // LIBRARIANPORTAL_H
