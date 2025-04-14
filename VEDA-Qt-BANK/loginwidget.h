#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class LoginWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    
signals:
    void loginRequested(const QString &username, const QString &password);
    
private slots:
    void onLoginButtonClicked();
    
private:
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QLabel *m_logoLabel;
    QLabel *m_titleLabel;
};

#endif // LOGINWIDGET_H
