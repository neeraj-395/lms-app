#include "models/bookmanager.h"
#include "ui_bookmanager.h"

#include <QMessageBox>

#include "utils/qutils.h"
#include "models/datatable.h"

namespace BookQueries 
{
    static const char* AddBook = R"(
        INSERT INTO books(title, author, isbn, available) 
        VALUES(:title, :author, :isbn, :is_available)
    )";
    static const char* RemoveBook = R"(
        DELETE FROM books WHERE id = :book_id
    )";
    static const char* SearchBook  = R"(
        SELECT *FROM books WHERE title LIKE :title AND 
        author LIKE :author AND isbn LIKE :isbn
    )";
    static const char* EditBook = R"(
        UPDATE books SET title = :title, author = :author, 
        isbn = :isbn, available = :is_available WHERE id = :book_id
    )";
    static const char* LoadBook = R"(
        SELECT *FROM books WHERE id = :book_id LIMIT 1
    )";
}

BookManager::BookManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BookManager)
{
    ui->setupUi(this);

    lineEditList = {
        ui->bookIdLineEdit,
        ui->titleLineEdit,   // The order of these elements is arranged
        ui->authorLineEdit, // according to the database schema.
        ui->ISBNLineEdit
    };

    connect(ui->loadBookBtn,  &QPushButton::clicked,
            this, &BookManager::loadBookData);
}

BookManager::~BookManager()
{
    delete ui;
}

void BookManager::initManager(const QString title, BookOp::Type type){
    QString textStyle = R"(<p align="center"><span style="font-size:20pt;">%1</span></p>)";
    ui->mainTitle->setText(textStyle.arg(title));
    operation = type;
    setupUiComponents();
    setupConnection();
    show();
}

void BookManager::searchBook(){

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::SearchBook,{
        {":isbn", "%" + ui->ISBNLineEdit->text() + "%"},
        {":title", "%" + ui->titleLineEdit->text() + "%"},
        {":author", "%" + ui->authorLineEdit->text() + "%"}
    });
    
    if(!result) {
        QMessageBox::critical(this, "Operation failed", "Database error occurred");
        return;
    }

    QList<QSqlRecord> data = query.getRecords();

    if (data.isEmpty()) {
        QMessageBox::information(this, "", "No books found matching the criteria.");
        return;
    }

    const char* headers[] = {"Book Id", "Title", "Author", "ISBN", "Available?"};
    DataTable *searchTable = new DataTable("Search Results");
    searchTable->populateTable(data, headers);
    searchTable->show();
    this->close();
}

void BookManager::addBook(){

    if(!UiUtils::validateLineEdits(lineEditList.mid(1))){
         QMessageBox::critical(this, "Empty Request", "Some fields are empty. "
                                        "Please fill them in to proceed.");
         return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::AddBook, {
        {":title", lineEditList.at(1)->text()},
        {":author", lineEditList.at(2)->text()},
        {":isbn", lineEditList.at(3)->text()},
        {":is_available", ui->availableCheckBox->checkState()}
    });
    
    if(!result || !query.numRowsAffected()) {
        QMessageBox::critical(this, "Operation failed", "Database error occurred");
        return;
    }

    UiUtils::setLineEditsText(lineEditList); // clear the form content
    ui->availableCheckBox->setCheckState(Qt::Unchecked);
}

void BookManager::removeBook() {

    if(ui->bookIdLineEdit->text().isEmpty()){
       QMessageBox::warning(this, "Empty Request", "Book Id field cannot be empty.");
       return ;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::RemoveBook, { 
        {":book_id", ui->bookIdLineEdit->text()} 
    });

    if(!result || !query.numRowsAffected()) {
        QMessageBox::critical(this, "Operation failed", "Database error occurred");
        return;
    }

    UiUtils::setLineEditsText(lineEditList); // clear the form content
    ui->availableCheckBox->setCheckState(Qt::Unchecked);
}

void BookManager::editBook(){

    if(!UiUtils::validateLineEdits(lineEditList)) {
         QMessageBox::warning(this, "Empty Request", "Some fields are empty. "
                                        "Please fill them in to proceed.");
         return;
    }

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::EditBook, {
        {":isbn", lineEditList.at(3)->text()},
        {":title", lineEditList.at(1)->text()},
        {":author", lineEditList.at(2)->text()},
        {":book_id", lineEditList.at(0)->text()},
        {":is_available", ui->availableCheckBox->checkState()}
    });
    
    if(!result || !query.numRowsAffected()) {
        QMessageBox::critical(this, "Operation failed", "Database error occurred");
        return;
    }

    UiUtils::setLineEditsText(lineEditList);
    ui->availableCheckBox->setCheckState(Qt::Unchecked);
}

void BookManager::loadBookData(){

    MySqlQuery query;

    bool result = query.execQuery(BookQueries::LoadBook, 
        {{":book_id", ui->bookIdLineEdit->text()}}
    );

    if(!result) {
        QMessageBox::critical(this, "Operation failed", "Database error occurred");
        return;
    }

    if (query.next()) {
        UiUtils::setLineEditsText(lineEditList, query.record());
        ui->availableCheckBox->setChecked(query.record().value(lineEditList.size()).toBool());
        QMessageBox::information(this, "Success", "Book data loaded successfully.");
    } else {
        UiUtils::setLineEditsText(lineEditList);
        ui->availableCheckBox->setCheckState(Qt::Unchecked);
        QMessageBox::critical(this, "Failed", "Book data not found.");
    }
}

void BookManager::setupUiComponents() {

    switch(operation) {

    case BookOp::SearchBook:
        ui->operationBtn->setText("Search");

        UiUtils::setFieldsEnabled(false, {
            ui->bookIdLineEdit,
            ui->loadBookBtn,
            ui->availableCheckBox
        });
        break;

    case BookOp::AddBook:
        ui->operationBtn->setText("Add");

        UiUtils::setFieldsEnabled(false, {
            ui->bookIdLineEdit,
            ui->loadBookBtn
        });
        break;

    case BookOp::RemoveBook:
        ui->operationBtn->setText("Remove");

        UiUtils::setFieldsEnabled(false, {
            ui->titleLineEdit,
            ui->ISBNLineEdit,
            ui->authorLineEdit,
            ui->availableCheckBox
        });
        break;

    case BookOp::EditBook:
        ui->operationBtn->setText("Save");
        break;
    }
}

void BookManager::setupConnection(){

    switch(operation) {

    case BookOp::SearchBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::searchBook);
        break;

    case BookOp::AddBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::addBook);
        break;

    case BookOp::RemoveBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::removeBook);
        break;

    case BookOp::EditBook:
        connect(ui->operationBtn, &QPushButton::clicked,
                this, &BookManager::editBook);
        break;
    }
}

void BookManager::closeEvent(QCloseEvent *event) {

    // cleanUp form line edits input
    for (int i = 0; i < lineEditList.size(); ++i) lineEditList[i]->clear();
    ui->availableCheckBox->setCheckState(Qt::PartiallyChecked);

    // disconnect connection
    disconnect(ui->operationBtn,&QPushButton::clicked,this,nullptr);

    // set the default ui state.
    UiUtils::setFieldsEnabled(true, {
        ui->bookIdLineEdit,
        ui->titleLineEdit,
        ui->ISBNLineEdit,
        ui->authorLineEdit,
        ui->availableCheckBox,
        ui->loadBookBtn
    });

    QDialog::closeEvent(event);
}
