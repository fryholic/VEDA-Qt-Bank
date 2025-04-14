#include "dashboardwidget.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLocale>

AccountListItem::AccountListItem(const QVariantMap &accountData, QWidget *parent)
    : QWidget(parent), m_accountNumber(accountData["accountNumber"].toString())

    // 계좌 아이콘 (첫 글자를 원 안에   m_accountNumber(accountData["accountNumber"].toString())
{
    // 계좌 아이콘 (첫 글자를 원 안에 표시)
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setFixedSize(40, 40);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setObjectName("accountIcon");
    iconLabel->setText(accountData["accountName"].toString().left(1));
    
    // 계좌 정보 레이블
    m_accountNameLabel = new QLabel(accountData["accountName"].toString(), this);
    m_accountNameLabel->setObjectName("accountName");
    
    m_accountNumberLabel = new QLabel(accountData["accountNumber"].toString(), this);
    m_accountNumberLabel->setObjectName("accountNumber");
    
    // 잔액 레이블
    QLocale locale = QLocale(QLocale::Korean);
    QString balanceText = locale.toString(accountData["balance"].toDouble(), 'f', 0) + " 원";
    m_balanceLabel = new QLabel(balanceText, this);
    m_balanceLabel->setObjectName("accountBalance");
    m_balanceLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    // 화살표 아이콘
    QLabel *arrowLabel = new QLabel(this);
    QPixmap arrowIcon(":/images/arrow_right.png");
    arrowLabel->setPixmap(arrowIcon.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    // 레이아웃 설정
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->addWidget(m_accountNameLabel);
    infoLayout->addWidget(m_accountNumberLabel);
    infoLayout->setSpacing(5);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(iconLabel);
    mainLayout->addLayout(infoLayout, 1);
    mainLayout->addWidget(m_balanceLabel);
    mainLayout->addWidget(arrowLabel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    
    // 스타일 설정
    setObjectName("accountListItem");
    setCursor(Qt::PointingHandCursor);
}

void AccountListItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(m_accountNumber);
    }
    QWidget::mousePressEvent(event);
}

DashboardWidget::DashboardWidget(QWidget *parent) : QWidget(parent)
{
    // 상단 헤더 영역
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setObjectName("dashboardHeader");
    
    m_usernameLabel = new QLabel(this);
    m_usernameLabel->setObjectName("usernameLabel");
    
    QLabel *totalBalanceTitle = new QLabel("총 계좌 금액", this);
    totalBalanceTitle->setObjectName("totalBalanceTitle");
    
    m_totalBalanceLabel = new QLabel(this);
    m_totalBalanceLabel->setObjectName("totalBalanceLabel");
    
    QVBoxLayout *userInfoLayout = new QVBoxLayout();
    userInfoLayout->addWidget(m_usernameLabel);
    userInfoLayout->addSpacing(5);
    
    QVBoxLayout *balanceLayout = new QVBoxLayout();
    balanceLayout->addWidget(totalBalanceTitle);
    balanceLayout->addWidget(m_totalBalanceLabel);
    balanceLayout->setAlignment(Qt::AlignRight);
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->addLayout(userInfoLayout);
    headerLayout->addStretch();
    headerLayout->addLayout(balanceLayout);
    headerLayout->setContentsMargins(20, 20, 20, 20);
    
    // 계좌 목록 영역
    QLabel *accountListTitle = new QLabel("전체 계좌 리스트", this);
    accountListTitle->setObjectName("sectionTitle");
    
    m_accountListWidget = new QListWidget(this);
    m_accountListWidget->setObjectName("accountList");
    m_accountListWidget->setFrameShape(QFrame::NoFrame);
    m_accountListWidget->setSpacing(10);
    
    // 로그아웃 버튼
    m_logoutButton = new QPushButton("로그아웃", this);
    m_logoutButton->setObjectName("secondaryButton");
    
    // 메인 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(headerWidget);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(accountListTitle);
    mainLayout->addWidget(m_accountListWidget, 1);
    
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_logoutButton);
    
    mainLayout->addLayout(bottomLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // 시그널-슬롯 연결
    connect(m_logoutButton, &QPushButton::clicked, this, &DashboardWidget::logoutRequested);
}

void DashboardWidget::updateUserInfo(const QString &username, double totalBalance)
{
    m_usernameLabel->setText(username + " 님, 안녕하세요!");
    
    QLocale locale = QLocale(QLocale::Korean);
    m_totalBalanceLabel->setText(locale.toString(totalBalance, 'f', 0) + " 원");
}

void DashboardWidget::updateAccountList(const QVariantList &accounts)
{
    m_accountListWidget->clear();
    
    for (const QVariant &account : accounts) {
        QVariantMap accountMap = account.toMap();
        
        QListWidgetItem *item = new QListWidgetItem(m_accountListWidget);
        AccountListItem *accountItem = new AccountListItem(accountMap, m_accountListWidget);
        
        item->setSizeHint(accountItem->sizeHint());
        m_accountListWidget->setItemWidget(item, accountItem);
        
        connect(accountItem, &AccountListItem::clicked, this, &DashboardWidget::accountSelected);
    }
}
