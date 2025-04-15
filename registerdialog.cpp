#include "registerdialog.h"
#include <QVBoxLayout>
#include <QLabel>

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("회원가입");

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("아이디");

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("비밀번호");
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_registerButton = new QPushButton("회원가입", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("새로운 계정을 입력하세요:"));
    layout->addWidget(m_usernameEdit);
    layout->addWidget(m_passwordEdit);
    layout->addWidget(m_registerButton);

    connect(m_registerButton, &QPushButton::clicked, this, &RegisterDialog::onRegisterClicked);
}

void RegisterDialog::onRegisterClicked()
{
    QString username = m_usernameEdit->text();
    QString password = m_passwordEdit->text();

    if (!username.isEmpty() && !password.isEmpty()) {
        emit registerUser(username, password);
        accept(); // 창 닫기
    }
}
