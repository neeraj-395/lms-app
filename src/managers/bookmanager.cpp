#include "managers/bookmanager.h"
#include "ui_bookmanager.h"

#include <QMessageBox>

#include "utils/qutils.h" // Alerts namepace, QUtils namespace
#include "utils/bookqueries.h"
#include "database/mysqlquery.h"

BookManager::BookManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BookManager)
{
    ui->setupUi(this);

    connect(ui->loadBookBtn,  &QPushButton::clicked,
    this, &BookManager::handleLoadBookDataRequest);
}

BookManager::~BookManager()
{
    delete ui;
}

void BookManager::initManager(const QString &title, BookOp::Type type)
{
    QString textStyle = R"(<p align="center">
                            <span style="font-size:20pt;">%1</span>
                           </p>)";
    ui->mainTitle->setText(textStyle.arg(title));
    operation = type;
    setupUiComponents();
    setupConnection();
    show();
}

void BookManager::handleSearchBookRequest()
{
    MySqlQuery query;

    bool result = query.execQuery(BookQueries::searchBook(), {
        {":isbn", "%" + ui->ISBNLineEdit->text() + "%"},
        {":title", "%" + ui->titleLineEdit->text() + "%"},
        {":author", "%" + ui->authorLineEdit->text() + "%"}
    });

    if(!result) { emit internalServiceError(); return; }

    if (!query.next()) {
        Alerts::info(this, "No books found matching the criteria.");
        return;
    }

    QSqlQueryModel *model = query.getSqlQueryModel();

    if(!model) emit internalServiceError();

    QUtils::showTableViewDialog(this, "Search Results", model);
}

void BookManager::handleAddBookRequest()
{
    if(!QUtils::validateLineEdits(getLineEdits().mid(1))) {
         Alerts::critical(this, "Some fields are empty. "
                                "Please fill them in to proceed.");
         return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::addBook(), {
        {":title",ui->titleLineEdit->text()},
        {":isbn",ui->authorLineEdit->text()},
        {":author",ui->authorLineEdit->text()},
        {":quantity",ui->quantityLineEdit->text()}
    });
    
    if(!result || !query.numRowsAffected()) {
        emit internalServiceError();
        return;
    }

    Alerts::info(this, "The book has been successfully "
                       "added to the library database!");
    QUtils::clearLineEdits(getLineEdits());
}


void BookManager::handleRemoveBookRequest()
{
    if(ui->bookIdLineEdit->text().isEmpty()) {
       Alerts::warn(this, "Book Id field can't be empty.");
       return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::removeBook(), {
        {":book_id", ui->bookIdLineEdit->text().toUInt()}
    });

    if(!result || !query.numRowsAffected()) {
        emit internalServiceError();
        return;
    }
    Alerts::info(this, "Book removal successful! The book is "
                       "no longer available in the library.");
    QUtils::clearLineEdits(getLineEdits()); // clear the form content
}

void BookManager::handleEditBookRequest()
{
    if(!QUtils::validateLineEdits(getLineEdits())) {
         Alerts::warn(this, "Some fields are empty. "
                            "Please fill them in to proceed.");
         return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::editBook(), {
        {":isbn",ui->ISBNLineEdit->text()},
        {":title",ui->titleLineEdit->text()},
        {":author",ui->authorLineEdit->text()},
        {":quantity",ui->quantityLineEdit->text()},
        {":book_id",ui->bookIdLineEdit->text().toUInt()}
    });
    
    if(!result || !query.numRowsAffected()) {
        emit internalServiceError();
        return;
    }
    Alerts::info(this, "Book edit successful! The updated "
                       "details are now saved.");
    QUtils::clearLineEdits(getLineEdits());
}

void BookManager::handleLoadBookDataRequest()
{
    MySqlQuery query;

    bool result = query.execQuery(BookQueries::loadBook(), {
        {":book_id", ui->bookIdLineEdit->text().toUInt()}
    });

    if(!result) { emit internalServiceError(); return; }

    if (query.next()) {
        QUtils::setLineEditsText(getLineEdits(), query.record());
        Alerts::info(this, "Book data loaded successfully.");
    } else {
        QUtils::clearLineEdits(getLineEdits());
        Alerts::warn(this, "Book data was not found.");
    }
}

void BookManager::setupUiComponents()
{
    switch(operation) {

    case BookOp::SearchBook:
        ui->operationBtn->setText("Search");
        ui->loadBookBtn->setEnabled(false);
        ui->bookIdLineEdit->setEnabled(false);
        ui->quantityLineEdit->setEnabled(false);
        break;

    case BookOp::AddBook:
        ui->operationBtn->setText("Add");
        ui->loadBookBtn->setEnabled(false);
        ui->bookIdLineEdit->setEnabled(false);
        break;

    case BookOp::RemoveBook:
        ui->operationBtn->setText("Remove");
        ui->ISBNLineEdit->setEnabled(false);
        ui->titleLineEdit->setEnabled(false);
        ui->authorLineEdit->setEnabled(false);
        ui->quantityLineEdit->setEnabled(false);
        break;

    case BookOp::EditBook:
        ui->operationBtn->setText("Save");
        break;
    }
}

void BookManager::setupConnection()
{
    switch(operation) {

    case BookOp::SearchBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::handleSearchBookRequest);
        break;

    case BookOp::AddBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::handleAddBookRequest);
        break;

    case BookOp::RemoveBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::handleRemoveBookRequest);
        break;

    case BookOp::EditBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::handleEditBookRequest);
        break;
    }
}

QList<QLineEdit*> BookManager::getLineEdits()
{
    return {ui->bookIdLineEdit,
            ui->titleLineEdit,   // The order of these elements is arranged
            ui->authorLineEdit, // according to the database schema.
            ui->ISBNLineEdit,
            ui->quantityLineEdit};
}

void BookManager::closeEvent(QCloseEvent *event)
{
    // cleanUp form line edits input
    QUtils::clearLineEdits(getLineEdits());

    // disconnect connection
    disconnect(ui->operationBtn,&QPushButton::clicked, this, nullptr);

    // set the default ui state.
    ui->bookIdLineEdit->setEnabled(true);
    ui->titleLineEdit->setEnabled(true);
    ui->ISBNLineEdit->setEnabled(true);
    ui->authorLineEdit->setEnabled(true);
    ui->quantityLineEdit->setEnabled(true);
    ui->loadBookBtn->setEnabled(true);

    QDialog::closeEvent(event);
}
