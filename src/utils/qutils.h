#ifndef QUTILS_H
#define QUTILS_H

#include <QList>
#include <QLineEdit>
#include <QWidget>
#include <QSqlRecord>
#include <QMessageBox>
#include <QAbstractItemModel>

namespace QUtils
{
    bool validateLineEdits(const QList<QLineEdit*> &lineEdits);
    void clearLineEdits(QList<QLineEdit*> lineEdits);
    void setLineEditsReadOnly(QList<QLineEdit*> lineEdits, bool readOnly);
    void setLineEditsText(const QList<QLineEdit*> &lineEdits, const QSqlRecord &data);
    void showTableViewDialog(QWidget *parent, const QString &title, QAbstractItemModel *model);
    //template<typename... LineEdits>
    //void clearLineEdits(LineEdits*... lineEdits){ (lineEdits->clear(), ...); }
}

namespace Alerts {
    void warn(QWidget *parent, const QString &message);
    void info(QWidget *parent, const QString &message);
    void critical(QWidget *parent, const QString &message);
}

#endif // QUTILS_H
