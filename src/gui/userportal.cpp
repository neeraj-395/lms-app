#include "gui/userportal.h"
#include "ui_userportal.h"

#include "utils/qutils.h"
#include "utils/userqueries.h"
#include "database/mysqlquery.h"

#include <QMessageBox>

UserPortal::UserPortal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserPortal)
    , book_ui(new BookManager(this))
{
    ui->setupUi(this);
    setupConnections();
}

UserPortal::~UserPortal()
{
    delete ui;
}

void UserPortal::handleLogoutRequest()
{
    emit logoutRequestToMain();
}

void UserPortal::handlebookSearchRequest()
{
    book_ui->initManager("Search for Books", BookOp::SearchBook);
}


void UserPortal::handleBookBorrowedRequest()
{
    MySqlQuery query;

    bool result = query.execQuery(UserQueries::bookBorrowed(user_id));
    if (!result) { handleInternalServiceError(); return; }

    if(!query.next()) {
        Alerts::info(this, "No book were borrowed by you yet!");
        return;
    }
}


void UserPortal:: handleFineRecordRequest()
{
    MySqlQuery query;

    bool result = query.execQuery(UserQueries::fineDetails(user_id));
    if (!result) { handleInternalServiceError(); return; }

    if(!query.next()){
        Alerts::info(this, "No due fine is pending by you!");
        return;
    }
}

void UserPortal::handleInternalServiceError()
{
    Alerts::critical(this, "The current opeartion failed due "
                           "to an internal service error.");
}

void UserPortal::setUser(const uint id, const QString &name){
    this->user_id = id;
    this->user_name = name;
    ui->logoutBox->setTitle(name);
}

void UserPortal::setupConnections()
{
    connect(ui->logoutBtn, &QPushButton::clicked, this, &UserPortal::handleLogoutRequest);

    connect(ui->searchBookBtn, &QPushButton::clicked, this, &UserPortal::handlebookSearchRequest);

    connect(ui->fineDetailBtn, &QPushButton::clicked, this, &UserPortal::handleFineRecordRequest);

    connect(ui->bookBorrowedBtn, &QPushButton::clicked, this, &UserPortal::handleBookBorrowedRequest);

    connect(book_ui, &BookManager::internalServiceError, this, &UserPortal::handleInternalServiceError);
}
