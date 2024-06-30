#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "database/mysqldb.h"
#include "gui/loginform.h"
#include "gui/userportal.h"
#include "gui/librarianportal.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showMainInterface();
    void showLoginInterface(const QString &title, Acc::Type type);
    void handleLogin(Acc::Type type, uint id, const QString name);

protected:
    using QWidget::show;

private:
    MySqlDatabase    db;
    Ui::MainWindow  *ui;
    QStackedWidget  *widgets;
    UserPortal      *user_ui;
    LoginForm       *login_ui;
    LibrarianPortal *librarian_ui;

private:
    void setupConnections();
    void setupStackedWidgets();
    void setupDatabaseConnection();
    void alertConnectionError(const QString &message);
};

#endif // MAINWINDOW_H
