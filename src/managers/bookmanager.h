#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include <QList>
#include <QDialog>
#include <QLineEdit>

namespace BookOp
{
    enum Type   {
        AddBook,
        RemoveBook,
        SearchBook,
        EditBook
    };
}

namespace Ui
{
    class BookManager;
}

class BookManager : public QDialog
{
    Q_OBJECT

public:
    explicit BookManager(QWidget *parent = nullptr);
    ~BookManager();

    void initManager(const QString &title, BookOp::Type type);

private slots:
    void handleAddBookRequest();
    void handleEditBookRequest();
    void handleSearchBookRequest();
    void handleRemoveBookRequest();
    void handleLoadBookDataRequest();

signals:
    void internalServiceError();

protected:
    using QWidget::show;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::BookManager   *ui;
    BookOp::Type      operation;

private:
    void setupConnection();
    void setupUiComponents();
    QList<QLineEdit*> getLineEdits();
};

#endif // BOOKMANAGER_H
