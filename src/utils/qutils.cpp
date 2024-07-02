#include "utils/qutils.h"

#include <QTableView>
#include <QScrollBar>
#include <QHeaderView>
#include <QVBoxLayout>

void Alerts::warn(QWidget *parent, const QString &message) {
    QMessageBox::warning(parent, "Invalid Request", message, QMessageBox::Ok);
}

void Alerts::info(QWidget *parent, const QString &message) {
    QMessageBox::information(parent, "Operation success", message, QMessageBox::Ok);
}

void Alerts::critical(QWidget *parent, const QString &message) {
    QMessageBox::critical(parent, "Operation failed", message, QMessageBox::Ok);
}

void QUtils::clearLineEdits(QList<QLineEdit*> lineEdits)
{
    for (QLineEdit* lineEdit : lineEdits) lineEdit->clear();
}

void QUtils::setLineEditsReadOnly(QList<QLineEdit*> lineEdits, bool readOnly)
{
    for (QLineEdit* lineEdit : lineEdits) lineEdit->setReadOnly(readOnly);
}

bool QUtils::validateLineEdits(const QList<QLineEdit*> &list)
{
    if(list.isEmpty()) return false;
    for(QLineEdit* LE : list) {
        if(LE && LE->text().isEmpty()) return false;
    } return true;
}

void QUtils::setLineEditsText(const QList<QLineEdit*> &list, const QSqlRecord &data)
{
    for (int i=0; i < list.size(); ++i) list[i]->setText(data.value(i).toString());
}

void QUtils::showTableViewDialog(QWidget *parent, const QString &title,
                                 QAbstractItemModel *model)
{
    QDialog *dialog = new QDialog(parent);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->setWindowTitle(title);

    QTableView *view = new QTableView(dialog);
    model->setParent(view);
    view->setModel(model);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->verticalHeader()->setVisible(false);
    view->setAlternatingRowColors(true);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(view);
    dialog->setLayout(layout);

    // Resize dialog to fit table view content
    view->resizeColumnsToContents();
    view->resizeRowsToContents();

    int width = view->verticalHeader()->width()
              + view->horizontalHeader()->length();

    int height = view->horizontalHeader()->height()
               + view->verticalHeader()->length();

    if (view->verticalScrollBar()->isVisible()) {
        width += view->verticalScrollBar()->width();
    }
    if (view->horizontalScrollBar()->isVisible()) {
        height += view->horizontalScrollBar()->height();
    }

    dialog->resize(width, height);
    dialog->show();
}
