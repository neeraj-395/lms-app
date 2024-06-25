#include "gui/librarianportal.h"
#include "ui_librarianportal.h"

LibrarianPortal::LibrarianPortal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibrarianPortal)
    , book_ui(new BookManager(this))
{
    ui->setupUi(this);
    setupConnection();
}

LibrarianPortal::~LibrarianPortal()
{
    delete ui;
}

void LibrarianPortal::setLibrarian(quint8 id, QString name){
    this->librarian_id = id;
    this->librarian_name = name;
    this->ui->logoutBox->setTitle(name);
}

void LibrarianPortal::setupConnection() {

    connect(ui->addNewBook, &QPushButton::clicked, this, [this](){
        book_ui->initManager("Add Book", BookOp::AddBook);
    });
    connect(ui->removeBook, &QPushButton::clicked, this, [this](){
        book_ui->initManager("Remove Book", BookOp::RemoveBook);
    });
    connect(ui->editBookDataBtn, &QPushButton::clicked, this, [this](){
        book_ui->initManager("Edit Book", BookOp::EditBook);
    });
    connect(ui->bookSearchBtn, &QPushButton::clicked, this, [this](){
        book_ui->initManager("Search For Books", BookOp::SearchBook);
    });
    connect(ui->logoutBtn, &QPushButton::clicked,this, &LibrarianPortal::backToMain);
}
