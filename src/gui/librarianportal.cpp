#include "gui/librarianportal.h"
#include "ui_librarianportal.h"

#include "utils/qutils.h"

LibrarianPortal::LibrarianPortal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibrarianPortal)
    , book_wd(new BookManager(this))
    , checkout_wd(new TransacManager(TransacOp::CheckoutBook, this))
    , return_wd(new TransacManager(TransacOp::ReturnBook, this))
{
    ui->setupUi(this);
    setupConnection();
}

LibrarianPortal::~LibrarianPortal()
{
    delete ui;
}

void LibrarianPortal::handleLogoutRequest()
{
    emit logoutRequestToMain();
}

void LibrarianPortal::setLibrarian(const uint id, const QString &name)
{
    this->librarian_id = id;
    this->librarian_name = name;
    this->ui->logoutBox->setTitle(name);
}

void LibrarianPortal::handleInternalServiceError()
{
    Alerts::critical(this, "The current opeartion failed due "
                           "to an internal service error.");
}

void LibrarianPortal::setupConnection() {

    connect(ui->addNewBookBtn, &QPushButton::clicked, this, [this](){
        book_wd->initManager("Add Book", BookOp::AddBook);
    });
    connect(ui->removeBookBtn, &QPushButton::clicked, this, [this](){
        book_wd->initManager("Remove Book", BookOp::RemoveBook);
    });
    connect(ui->editBookDataBtn, &QPushButton::clicked, this, [this](){
        book_wd->initManager("Edit Book", BookOp::EditBook);
    });
    connect(ui->bookSearchBtn, &QPushButton::clicked, this, [this](){
        book_wd->initManager("Search for Books", BookOp::SearchBook);
    });

    connect(ui->checkoutBookBtn, &QPushButton::clicked, this, [this](){
        checkout_wd->show();
    });

    connect(ui->returnBookBtn, &QPushButton::clicked, this, [this](){
        return_wd->show();
    });

    connect(checkout_wd, &TransacManager::internalServiceError,
    this, &LibrarianPortal::handleInternalServiceError);

    connect(return_wd, &TransacManager::internalServiceError,
    this, &LibrarianPortal::handleInternalServiceError);

    connect(book_wd, &BookManager::internalServiceError,
    this, &LibrarianPortal::handleInternalServiceError);

    connect(ui->logoutBtn, &QPushButton::clicked,
    this, &LibrarianPortal::handleLogoutRequest);
}
