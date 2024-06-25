#include <QMessageBox>

#include "gui/loginform.h"
#include "ui_loginform.h"
#include "database/mysqlquery.h"

LoginForm::LoginForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginForm::loginHandler);
    connect(ui->backBtn, &QPushButton::clicked, this, &LoginForm::backToMain);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::loginHandler()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    MySqlQuery query;

    bool result = query.execQuery(getQuery(), { {":name", username} });

    if(!result) {
        QMessageBox::critical(this, "Database failure", "Unable to execute query.");
        return;
    }

    if(!query.next() || query.record().value(1).toString() != password) {
        QMessageBox::warning(this, "Login failed", "Incorrect username or password.");
        return;
    }

    quint8 id = query.record().value(0).toUInt();

    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();

    emit loginSuccess(accType, id, username);
}

void LoginForm::setLoginUi(const QString title, Acc::Type type) {
    QString titleStyle = R"(
        <p align="center">
            <span style="font-size:28pt; font-weight:400;">%1</span>
        </p>
    )";
    this->ui->mainTitle->setText(titleStyle.arg(title));
    this->accType = type;
}

QString LoginForm::getQuery() {
    switch(accType) {
    case Acc::User:
        return "SELECT id, password FROM users WHERE name = :name";
    case Acc::Librarian:
        return "SELECT id, password FROM librarians WHERE name = :name";
    default:
        return "";
    }
}

