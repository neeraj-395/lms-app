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

    lineEditList = {
        ui->bookIdLineEdit,
        ui->titleLineEdit,   // The order of these elements is arranged
        ui->authorLineEdit, // according to the database schema.
        ui->ISBNLineEdit,
        ui->quantityLineEdit
    };

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
    if(!QUtils::validateLineEdits(lineEditList.mid(1))) {
         Alerts::critical(this, "Some fields are empty. "
                                "Please fill them in to proceed.");
         return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::addBook(), {
        {":title", lineEditList.at(1)->text()},
        {":author", lineEditList.at(2)->text()},
        {":isbn", lineEditList.at(3)->text()},
        {":quantity", lineEditList.at(4)->text()}
    });
    
    if(!result || !query.numRowsAffected()) {
        emit internalServiceError();
        return;
    }

    Alerts::info(this, "The book has been successfully "
                       "added to the library database!");
    QUtils::clearLineEdits(lineEditList);
}


void BookManager::handleRemoveBookRequest()
{
    if(ui->bookIdLineEdit->text().isEmpty()) {
       Alerts::warn(this, "Book Id field cannot be empty.");
       return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::removeBook(), {
        {":book_id", ui->bookIdLineEdit->text()} 
    });

    if(!result || !query.numRowsAffected()) {
        emit internalServiceError();
        return;
    }
    Alerts::info(this, "Book removal successful! The book is "
                       "no longer available in the library.");
    QUtils::clearLineEdits(lineEditList); // clear the form content
}

void BookManager::handleEditBookRequest()
{
    if(!QUtils::validateLineEdits(lineEditList)) {
         Alerts::warn(this, "Some fields are empty. Please "
                            "fill them in to proceed.");
         return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::editBook(), {
        {":isbn", lineEditList.at(3)->text()},
        {":title", lineEditList.at(1)->text()},
        {":author", lineEditList.at(2)->text()},
        {":book_id", lineEditList.at(0)->text()},
        {":quantity", lineEditList.at(4)->text()}
    });
    
    if(!result || !query.numRowsAffected()) {
        emit internalServiceError();
        return;
    }
    Alerts::info(this, "Book edit successful! The updated "
                       "details are now saved.");
    QUtils::clearLineEdits(lineEditList);
}

void BookManager::handleLoadBookDataRequest()
{
    MySqlQuery query;

    bool result = query.execQuery(BookQueries::loadBook(), {
        {":book_id", ui->bookIdLineEdit->text()}
    });

    if(!result) { emit internalServiceError(); return; }

    if (query.next()) {
        QUtils::setLineEditsText(lineEditList, query.record());
        Alerts::info(this, "Book data loaded successfully.");
    } else {
        QUtils::clearLineEdits(lineEditList);
        Alerts::warn(this, "Book data was not found.");
    }
}

void BookManager::setupUiComponents()
{
    switch(operation) {

    case BookOp::SearchBook:
        ui->operationBtn->setText("Search");

        QUtils::setWidgetsEnabled({
            ui->bookIdLineEdit,
            ui->loadBookBtn,
            ui->quantityLineEdit
        }, false);

        break;

    case BookOp::AddBook:
        ui->operationBtn->setText("Add");

        QUtils::setWidgetsEnabled({
            ui->bookIdLineEdit,
            ui->loadBookBtn
        }, false);

        break;

    case BookOp::RemoveBook:
        ui->operationBtn->setText("Remove");

        QUtils::setWidgetsEnabled({
            ui->titleLineEdit,
            ui->ISBNLineEdit,
            ui->authorLineEdit,
            ui->quantityLineEdit
        }, false);

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

void BookManager::closeEvent(QCloseEvent *event)
{
    // cleanUp form line edits input
    QUtils::clearLineEdits(lineEditList);

    // disconnect connection
    disconnect(ui->operationBtn,&QPushButton::clicked, this, nullptr);

    // set the default ui state.
    QUtils::setWidgetsEnabled({
        ui->bookIdLineEdit,
        ui->titleLineEdit,
        ui->ISBNLineEdit,
        ui->authorLineEdit,
        ui->quantityLineEdit,
        ui->loadBookBtn
    }, true);

    QDialog::closeEvent(event);
}
