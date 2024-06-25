#ifndef LIBRARIANPORTAL_H
#define LIBRARIANPORTAL_H

#include <QWidget>
#include "models/bookmanager.h"

namespace Ui { class LibrarianPortal; }

class LibrarianPortal : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianPortal(QWidget *parent = nullptr);
    ~LibrarianPortal();

    void setLibrarian(quint8 id, QString name);

signals:
    void backToMain();

private:
    Ui::LibrarianPortal *ui;
    BookManager         *book_ui;
    quint8              librarian_id;
    QString             librarian_name;

private:
    void setupConnection();
};

#endif // LIBRARIANPORTAL_H
