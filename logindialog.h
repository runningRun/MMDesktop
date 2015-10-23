#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class LoginDialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
signals:
    void btnLoginClicked();
private slots:
    void onLoginButtonClicked();
private:
    Ui::LoginDialog *ui;
};

#endif // DIALOG_H
