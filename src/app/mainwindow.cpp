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

void MainWindow::handleConnectionError(const QString &message) 
{
    QMessageBox::critical(this, "Connection failed", message, QMessageBox::Ok);
}

void MainWindow::onLoginSuccess(Acc::Type type, quint8 id, QString name) 
{
    switch(type) {
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

void MainWindow::setupStackedWidgets() 
{
    widgets = new QStackedWidget(this);
    widgets->addWidget(ui->centralwidget);
    widgets->addWidget(login_ui);
    widgets->addWidget(user_ui);
    widgets->addWidget(librarian_ui);
    setCentralWidget(widgets);
}

void MainWindow::setupDatabaseConnection() 
{
    if(!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        QMessageBox::critical(this,
                              "Connection failed",
                              "Database driver QMYSQL is not available.",
                              QMessageBox::Ok);
        return;
    }

    QString config_file("db.config.ini");
    QFile check_file(config_file);
    if(!check_file.exists()) {
        QMessageBox::critical(this,
                              "Connection failed",
                              "File db.config.ini is missing from"
                              " the executable diretory",
                              QMessageBox::Ok);
        return;
    }
    
    QSettings settings(config_file, QSettings::IniFormat);
    if(!db.connect(settings.value("DB_NAME").toString(),
                   settings.value("DB_HOSTNAME").toString(),
                   settings.value("DB_USERNAME").toString(),
                   settings.value("DB_PASSWORD").toString(),
                   settings.value("DB_PORT").toInt()))
    {
        QMessageBox::critical(this,
                              "Connection failed",
                              "Unable to open database connection",
                              QMessageBox::Ok);
        return;
    }
}

void MainWindow::setupConnections() 
{
    connect(login_ui, &LoginForm::loginSuccess,this, &MainWindow::onLoginSuccess);

    connect(login_ui, &LoginForm::backToMain, this, &MainWindow::showMainInterface);

    connect(user_ui, &UserPortal::backToMain, this, &MainWindow::showMainInterface);

    connect(librarian_ui, &LibrarianPortal::backToMain, this, &MainWindow::showMainInterface);

    connect(ui->userLogin, &QPushButton::clicked, this, [this]() {
        login_ui->setLoginUi("User Login", Acc::User);
        widgets->setCurrentWidget(login_ui);
    });

    connect(widgets, &QStackedWidget::currentChanged, this, [this]() {
        resize(widgets->currentWidget()->size());
    });

    connect(ui->librarianLogin, &QPushButton::clicked, this, [this]() {
        login_ui->setLoginUi("Librarian Login", Acc::Librarian);
        widgets->setCurrentWidget(login_ui);
    });
}


