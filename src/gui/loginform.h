#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

// Enum to represent the type of person in the library management system
namespace Acc
{
    enum Type { User, Librarian };
}

namespace Ui { class LoginForm; }

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

    void setLoginUi(const QString title, Acc::Type type);

signals:
    void backToMain();
    void loginSuccess(Acc::Type type, const uint id, const QString &name);

private slots:
    void loginHandler();

private:
    Ui::LoginForm *ui;
    Acc::Type     accType;
    QString       getQuery();
};

#endif // LOGINFORM_H
