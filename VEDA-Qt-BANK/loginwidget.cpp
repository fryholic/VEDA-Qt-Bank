#include "loginwidget.h"
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    // 로고 및 타이틀
    m_logoLabel = new QLabel(this);
    QPixmap logo(":/images/bank_logo.png");
    m_logoLabel->setPixmap(logo.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_logoLabel->setAlignment(Qt::AlignCenter);
    
    m_titleLabel = new QLabel("VEDA", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setObjectName("titleLabel");
    
    // 입력 필드
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("아이디");
    m_usernameEdit->setObjectName("loginInput");
    
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("비밀번호");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setObjectName("loginInput");
    
    // 로그인 버튼
    m_loginButton = new QPushButton("로그인", this);
    m_loginButton->setObjectName("primaryButton");
    
    // 레이아웃 설정
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(20);
    centerLayout->addWidget(m_logoLabel);
    centerLayout->addWidget(m_titleLabel);
    centerLayout->addWidget(m_usernameEdit);
    centerLayout->addWidget(m_passwordEdit);
    centerLayout->addWidget(m_loginButton);
    
    // 중앙 레이아웃을 가운데 정렬
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->addStretch();
    horizontalLayout->addLayout(centerLayout);
    horizontalLayout->addStretch();
    
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch();
    
    // 시그널-슬롯 연결
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginButtonClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWidget::onLoginButtonClicked);
}

void LoginWidget::onLoginButtonClicked()
{
    QString username = m_usernameEdit->text();
    QString password = m_passwordEdit->text();
    
    if (!username.isEmpty()) {
        emit loginRequested(username, password);
    }
}
