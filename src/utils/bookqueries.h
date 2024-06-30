#ifndef BOOKQUERIES_H
#define BOOKQUERIES_H

#include <QString>

namespace BookQueries
{
    inline QString addBook()
    {
        return QStringLiteral("INSERT INTO books(title, author, isbn, quantity) "
                              "VALUES(:title, :author, :isbn, :quantity)");
    }

    inline QString removeBook()
    {
        return QStringLiteral("DELETE FROM books WHERE id = :book_id");
    }

    inline QString searchBook()
    {
        return QStringLiteral("SELECT *FROM books WHERE title LIKE :title "
                              "AND author LIKE :author AND isbn LIKE :isbn");
    }

    inline QString editBook()
    {
        return QStringLiteral("UPDATE books SET title = :title, author = :author, "
                              "isbn = :isbn, quantity = :quantity WHERE id = :book_id");
    }

    inline QString loadBook()
    {
        return QStringLiteral("SELECT * FROM books WHERE id = :book_id LIMIT 1");
    }
}

#endif // BOOKQUERIES_H
