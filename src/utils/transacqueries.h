#ifndef TRANSACQUERIES_H
#define TRANSACQUERIES_H

#include <QString>

namespace TransacQueries
{
    inline QString selectBook(const uint &book_id)
    {
        return QStringLiteral("SELECT title, isbn, author FROM books "
                              "WHERE id=%1;").arg(book_id);
    }

    inline QString selectUser(const uint &user_id)
    {
        return QStringLiteral("SELECT name FROM users WHERE id=%1;"
                             ).arg(user_id);
    }

    inline QString selectBorrower(const uint &user_id, const uint &book_id)
    {
        return QStringLiteral("SELECT id, checkout_date, return_date "
                              "FROM borrowers WHERE user_id = %1 "
                              "AND book_id = %2 AND returned = FALSE;"
                              ).arg(user_id).arg(book_id);
    }
    inline QString incBookQuantity(const uint &book_id)
    {
        return QStringLiteral("UPDATE books SET quantity = quantity + 1 "
                              "WHERE id=%1;").arg(book_id);
    }

    inline QString decBookQuantity(const uint &book_id)
    {
        return QStringLiteral("UPDATE books SET quantity = quantity - 1 "
                              "WHERE id=%1;").arg(book_id);
    }

    inline QString updateReturned(const uint &borrower_id)
    {
        return QStringLiteral("UPDATE borrowers SET returned = TRUE WHERE id = %1;"
                              ).arg(borrower_id);
    }

    inline QString addBorrower()
    {
        return QStringLiteral("INSERT INTO borrowers (user_id, book_id, return_date)"
                              "VALUES (:user_id, :book_id, :return_date);");
    }

    inline QString addfine(const uint borr_id, const float fine_amount)
    {
        return QStringLiteral("INSERT INTO fines (borrower_id, fine_amount)"
                              "VALUES (%1, %2);").arg(borr_id).arg(fine_amount);
    }
}

#endif // TRANSACQUERIES_H
