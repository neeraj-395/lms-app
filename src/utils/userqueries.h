#ifndef USERQUERIES_H
#define USERQUERIES_H

#include <QString>

namespace UserQueries
{
    inline QString bookBorrowed(const uint &user_id)
    {
        return QStringLiteral(R"(
            SELECT b.title, b.author, b.isbn, bor.checkout_date,
            bor.return_by, bor.return_status FROM borrowers bor
            JOIN books b ON bor.book_id = b.id JOIN users u
            ON bor.user_id = u.id WHERE u.id = %1;
        )").arg(user_id);
    }

    inline QString fineDetails(const uint &user_id)
    {
        return QStringLiteral(R"(
            SELECT f.id, b.title, b.isbn, f.fine_amount, f.pay_status
            FROM fines f JOIN borrowers bor ON f.borrower_id = bor.id
            JOIN books b ON bor.book_id = b.id WHERE bor.user_id = %1;
        )").arg(user_id);
    }

    inline QString overdueBooks()
    {
        return QStringLiteral(R"(
            SELECT users.id AS user_id, users.name AS user_name, books.id AS book_id,
            books.title AS book_title, borrowers.checkout_date, borrowers.return_date
            FROM borrowers JOIN users ON borrowers.user_id = users.id JOIN books
            ON borrowers.book_id = books.id WHERE borrowers.return_date < CURRENT_DATE
            AND borrowers.returned = FALSE;
        )");
    }
}

#endif // USERQUERIES_H
