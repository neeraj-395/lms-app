#ifndef USERPORTAL_H
#define USERPORTAL_H

#include <QWidget>
#include <QSqlRecord>

#include "models/bookmanager.h"

namespace Ui { class UserPortal; }

class UserPortal : public QWidget
{
    Q_OBJECT

public:
    explicit UserPortal(QWidget *parent = nullptr);
    ~UserPortal();

    void setUser(quint8 id, QString name);

private slots:
    void bookSearch();
    void viewFineDetails();
    void viewBookBorrowed();

signals:
    void backToMain();

private:
    quint8 userId;
    QString userName;
    Ui::UserPortal *ui;
    BookManager *book_ui;
    
private:
    void setupConnections();
};

#endif // USERPORTAL_H
