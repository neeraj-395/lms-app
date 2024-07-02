#include "app/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , login_ui(new LoginForm(this))
    , user_ui(new UserPortal(this))
    , librarian_ui(new LibrarianPortal(this))
    , widgets(new QStackedWidget(this))
{
    ui->setupUi(this);
    
    setupStackedWidgets();

    setupConnections();

    this->show();

    setupDatabaseConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::alertConnectionError(const QString &message)
{
    QMessageBox::critical(this, "Connection failed", message, QMessageBox::Ok);
}

void MainWindow::handleLogin(Acc::Type type, const uint id, const QString name)
{
    switch(type)
    {
        case Acc::User:
            widgets->setCurrentWidget(user_ui);
            user_ui->setUser(id, name);
            break;

        case Acc::Librarian:
            widgets->setCurrentWidget(librarian_ui);
            librarian_ui->setLibrarian(id, name);
            break;
    }
}

void MainWindow::showMainInterface()
{
    widgets->setCurrentIndex(0);
}

void MainWindow::showLoginInterface(const QString &title, Acc::Type type)
{
    if(!db.checkConnection()) {
        alertConnectionError("Unable to open database connection. "
                             "Please try again later.");
        return;
    }
    login_ui->setLoginUi(title, type);
    widgets->setCurrentWidget(login_ui);
}

void MainWindow::setupStackedWidgets() 
{
    widgets->addWidget(ui->centralwidget);
    widgets->addWidget(login_ui);
    widgets->addWidget(user_ui);
    widgets->addWidget(librarian_ui);
    setCentralWidget(widgets);
}

void MainWindow::setupDatabaseConnection() 
{
    if(!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        alertConnectionError("Database driver QMYSQL is not available.");
        return;
    }

    QString config_file(":/res/config.db.ini");
    QFile check_file(config_file);
    if(!check_file.exists()) {
        alertConnectionError("Unable to find the configuration file (config.db.ini).");
        return;
    }
    
    QSettings settings(config_file, QSettings::IniFormat);
    if(!db.connect(settings.value("DB_NAME").toString(),
                   settings.value("DB_HOST").toString(),
                   settings.value("DB_USER").toString(),
                   settings.value("DB_PASS").toString(),
                   settings.value("DB_PORT").toInt()))
    {
        alertConnectionError("Unable to open database connection.");
        return;
    }
}

void MainWindow::setupConnections() 
{
    connect(ui->userLogin, &QPushButton::clicked, this, [this]() {
        showLoginInterface("User Login", Acc::User);
    });

    connect(ui->librarianLogin, &QPushButton::clicked, this, [this]() {
        showLoginInterface("Librarian Login", Acc::Librarian);
    });

    connect(widgets, &QStackedWidget::currentChanged, this, [this]() {
        resize(widgets->currentWidget()->size());
    });

    connect(login_ui, &LoginForm::loginSuccess,this, &MainWindow::handleLogin);

    connect(login_ui, &LoginForm::backToMain, this, &MainWindow::showMainInterface);

    connect(user_ui, &UserPortal::logoutRequestToMain, this, &MainWindow::showMainInterface);

    connect(librarian_ui, &LibrarianPortal::logoutRequestToMain, this, &MainWindow::showMainInterface);
}


