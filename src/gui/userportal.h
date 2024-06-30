#ifndef USERPORTAL_H
#define USERPORTAL_H

#include <QWidget>
#include <QSqlRecord>

#include "managers/bookmanager.h"

namespace Ui { class UserPortal; }

class UserPortal : public QWidget
{
    Q_OBJECT

public:
    explicit UserPortal(QWidget *parent = nullptr);
    ~UserPortal();

    void setUser(const uint id, const QString &name);

private slots:
    void handleLogoutRequest();
    void handlebookSearchRequest();
    void handleFineRecordRequest();
    void handleBookBorrowedRequest();
    void handleInternalServiceError();

signals:
    void logoutRequestToMain();

private:
    uint user_id;
    QString user_name;
    Ui::UserPortal *ui;
    BookManager *book_ui;
    
private:
    void setupConnections();
};

#endif // USERPORTAL_H
