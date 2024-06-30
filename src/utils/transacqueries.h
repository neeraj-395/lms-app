#ifndef TRANSACQUERIES_H
#define TRANSACQUERIES_H

#include <QString>

namespace TransacQueries
{
    inline QString selectBook(const uint &bookId)
    {
        return QStringLiteral("SELECT * FROM books WHERE id=%1 LIMIT 1").arg(bookId);
    }

    inline QString selectUser(const uint &userId)
    {
        return QStringLiteral("SELECT * FROM users WHERE id=%1 LIMIT 1").arg(userId);
    }

    inline QString selectBorr(const uint &userId, const uint &bookId)
    {
        return QStringLiteral("SELECT * FROM borrowers WHERE user_id=%1 "
                              "AND book_id=%2 AND returned=FALSE").arg(userId).arg(bookId);
    }
}

#endif // TRANSACQUERIES_H
