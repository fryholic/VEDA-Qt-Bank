#include "accountwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLocale>

TransactionListItem::TransactionListItem(const QVariantMap &transactionData, QWidget *parent)
    : QWidget(parent)
{
    // 거래 유형 아이콘
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setFixedSize(36, 36);
    iconLabel->setAlignment(Qt::AlignCenter);
    
    QString type = transactionData["type"].toString();
    if (type == "입금") {
        iconLabel->setObjectName("depositIcon");
        iconLabel->setText("+");
    } else if (type == "출금") {
        iconLabel->setObjectName("withdrawIcon");
        iconLabel->setText("-");
    } else {
        iconLabel->setObjectName("transferIcon");
        iconLabel->setText("→");
    }
    
    // 거래 정보 레이블
    m_descriptionLabel = new QLabel(transactionData["description"].toString(), this);
    m_descriptionLabel->setObjectName("transactionDescription");
    
    m_dateLabel = new QLabel(transactionData["date"].toString(), this);
    m_dateLabel->setObjectName("transactionDate");
    
    // 금액 레이블
    QLocale locale = QLocale(QLocale::Korean);
    QString prefix = (type == "출금" || type == "송금") ? "-" : "+";
    QString amountText = prefix + locale.toString(transactionData["amount"].toDouble(), 'f', 0) + " 원";
    
    m_amountLabel = new QLabel(amountText, this);
    m_amountLabel->setObjectName(type == "출금" || type == "송금" ? "withdrawAmount" : "depositAmount");
    m_amountLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    // 레이아웃 설정
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->addWidget(m_descriptionLabel);
    infoLayout->addWidget(m_dateLabel);
    infoLayout->setSpacing(5);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(iconLabel);
    mainLayout->addLayout(infoLayout, 1);
    mainLayout->addWidget(m_amountLabel);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    
    // 스타일 설정
    setObjectName("transactionListItem");
}

AccountWidget::AccountWidget(QWidget *parent) : QWidget(parent)
{
    // 상단 헤더 영역
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setObjectName("accountHeader");
    
    m_backButton = new QPushButton("뒤로", this);
    m_backButton->setObjectName("secondaryButton");
    
    QLabel *titleLabel = new QLabel("계좌 관리", this);
    titleLabel->setObjectName("pageTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->addWidget(m_backButton);
    headerLayout->addWidget(titleLabel, 1);
    headerLayout->addSpacerItem(new QSpacerItem(m_backButton->sizeHint().width(), 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
    headerLayout->setContentsMargins(20, 20, 20, 20);
    
    // 계좌 정보 영역
    QWidget *accountInfoWidget = new QWidget(this);
    accountInfoWidget->setObjectName("accountInfoCard");
    
    QLabel *accountInfoTitle = new QLabel("세부 계좌 정보", this);
    accountInfoTitle->setObjectName("cardTitle");
    
    m_accountNameLabel = new QLabel(this);
    m_accountNameLabel->setObjectName("accountDetailName");
    
    m_accountNumberLabel = new QLabel(this);
    m_accountNumberLabel->setObjectName("accountDetailNumber");
    
    m_balanceLabel = new QLabel(this);
    m_balanceLabel->setObjectName("accountDetailBalance");
    
    QVBoxLayout *accountInfoLayout = new QVBoxLayout(accountInfoWidget);
    accountInfoLayout->addWidget(accountInfoTitle);
    accountInfoLayout->addWidget(m_accountNameLabel);
    accountInfoLayout->addWidget(m_accountNumberLabel);
    accountInfoLayout->addWidget(m_balanceLabel);
    accountInfoLayout->setSpacing(8);
    accountInfoLayout->setContentsMargins(16, 16, 16, 16);
    
    // 거래 버튼 영역
    QHBoxLayout *transactionButtonsLayout = new QHBoxLayout();
    
    m_depositButton = new QPushButton("입금", this);
    m_depositButton->setObjectName("primaryButton");
    
    m_withdrawButton = new QPushButton("출금", this);
    m_withdrawButton->setObjectName("primaryButton");
    
    m_transferButton = new QPushButton("송금", this);
    m_transferButton->setObjectName("primaryButton");
    
    transactionButtonsLayout->addWidget(m_depositButton);
    transactionButtonsLayout->addWidget(m_withdrawButton);
    transactionButtonsLayout->addWidget(m_transferButton);
    transactionButtonsLayout->setSpacing(10);
    
    // 거래 내역 영역
    QLabel *transactionListTitle = new QLabel("입출금 내역", this);
    transactionListTitle->setObjectName("sectionTitle");
    
    m_transactionListWidget = new QListWidget(this);
    m_transactionListWidget->setObjectName("transactionList");
    m_transactionListWidget->setFrameShape(QFrame::NoFrame);
    m_transactionListWidget->setSpacing(8);
    
    // 메인 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(accountInfoWidget);
    mainLayout->addLayout(transactionButtonsLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(transactionListTitle);
    mainLayout->addWidget(m_transactionListWidget, 1);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // 시그널-슬롯 연결
    connect(m_backButton, &QPushButton::clicked, this, &AccountWidget::backRequested);
    connect(m_depositButton, &QPushButton::clicked, this, &AccountWidget::onDepositClicked);
    connect(m_withdrawButton, &QPushButton::clicked, this, &AccountWidget::onWithdrawClicked);
    connect(m_transferButton, &QPushButton::clicked, this, &AccountWidget::onTransferClicked);
}

void AccountWidget::updateAccountDetails(const QVariantMap &accountDetails)
{
    m_accountNumber = accountDetails["accountNumber"].toString();
    m_accountNameLabel->setText(accountDetails["accountName"].toString());
    m_accountNumberLabel->setText(accountDetails["accountNumber"].toString());
    
    QLocale locale = QLocale(QLocale::Korean);
    m_balanceLabel->setText(locale.toString(accountDetails["balance"].toDouble(), 'f', 0) + " 원");
}

void AccountWidget::updateTransactionList(const QVariantList &transactions)
{
    m_transactionListWidget->clear();
    
    if (transactions.isEmpty()) {
        QListWidgetItem *emptyItem = new QListWidgetItem(m_transactionListWidget);
        QLabel *emptyLabel = new QLabel("거래 내역이 없습니다. 첫 거래를 시작해보세요!", this);
        emptyLabel->setObjectName("emptyTransactionLabel");
        emptyLabel->setAlignment(Qt::AlignCenter);
        
        emptyItem->setSizeHint(QSize(m_transactionListWidget->width(), 100));
        m_transactionListWidget->setItemWidget(emptyItem, emptyLabel);
    } else {
        for (const QVariant &transaction : transactions) {
            QVariantMap transactionMap = transaction.toMap();
            
            QListWidgetItem *item = new QListWidgetItem(m_transactionListWidget);
            TransactionListItem *transactionItem = new TransactionListItem(transactionMap, m_transactionListWidget);
            
            item->setSizeHint(transactionItem->sizeHint());
            m_transactionListWidget->setItemWidget(item, transactionItem);
        }
    }
}

void AccountWidget::onDepositClicked()
{
    emit transactionRequested(m_accountNumber, "deposit");
}

void AccountWidget::onWithdrawClicked()
{
    emit transactionRequested(m_accountNumber, "withdraw");
}

void AccountWidget::onTransferClicked()
{
    emit transactionRequested(m_accountNumber, "transfer");
}
