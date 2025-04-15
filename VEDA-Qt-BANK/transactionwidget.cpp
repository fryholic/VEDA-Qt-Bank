#include "transactionwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLocale>
#include <QDoubleValidator>
#include <QRegularExpressionValidator>

TransactionWidget::TransactionWidget(QWidget *parent) : QWidget(parent)
{
    m_stackedWidget = new QStackedWidget(this);
    
    // 금액 입력 화면 설정
    m_amountInputWidget = new QWidget(this);
    
    // 상단 헤더
    QWidget *amountHeaderWidget = new QWidget(m_amountInputWidget);
    amountHeaderWidget->setObjectName("transactionHeader");
    
    m_amountBackButton = new QPushButton("뒤로", m_amountInputWidget);
    m_amountBackButton->setObjectName("secondaryButton");
    
    m_transactionTitleLabel = new QLabel(m_amountInputWidget);
    m_transactionTitleLabel->setObjectName("pageTitle");
    m_transactionTitleLabel->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout *amountHeaderLayout = new QHBoxLayout(amountHeaderWidget);
    amountHeaderLayout->addWidget(m_amountBackButton);
    amountHeaderLayout->addWidget(m_transactionTitleLabel, 1);
    amountHeaderLayout->addSpacerItem(new QSpacerItem(m_amountBackButton->sizeHint().width(), 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
    amountHeaderLayout->setContentsMargins(20, 20, 20, 20);
    
    // 금액 입력 영역
    QWidget *amountInputCard = new QWidget(m_amountInputWidget);
    amountInputCard->setObjectName("inputCard");
    
    QLabel *amountInputTitle = new QLabel("금액 입력", m_amountInputWidget);
    amountInputTitle->setObjectName("inputTitle");
    
    m_amountEdit = new QLineEdit(m_amountInputWidget);
    m_amountEdit->setPlaceholderText("금액을 입력하세요");
    m_amountEdit->setObjectName("amountInput");
    
    // 숫자만 입력 가능하도록 설정
    QDoubleValidator *validator = new QDoubleValidator(0, 4200000000, 0, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    m_amountEdit->setValidator(validator);
    
    m_amountErrorLabel = new QLabel(m_amountInputWidget);
    m_amountErrorLabel->setObjectName("errorLabel");
    m_amountErrorLabel->setVisible(false);
    
    m_limitInfoLabel = new QLabel(m_amountInputWidget);
    m_limitInfoLabel->setObjectName("infoLabel");
    
    QVBoxLayout *amountInputCardLayout = new QVBoxLayout(amountInputCard);
    amountInputCardLayout->addWidget(amountInputTitle);
    amountInputCardLayout->addWidget(m_amountEdit);
    amountInputCardLayout->addWidget(m_amountErrorLabel);
    amountInputCardLayout->addWidget(m_limitInfoLabel);
    amountInputCardLayout->setSpacing(10);
    amountInputCardLayout->setContentsMargins(16, 16, 16, 16);
    
    // 다음 버튼
    m_amountNextButton = new QPushButton("다음", m_amountInputWidget);
    m_amountNextButton->setObjectName("primaryButton");
    
    QVBoxLayout *amountLayout = new QVBoxLayout(m_amountInputWidget);
    amountLayout->addWidget(amountHeaderWidget);
    amountLayout->addWidget(amountInputCard);
    amountLayout->addStretch();
    amountLayout->addWidget(m_amountNextButton);
    amountLayout->setContentsMargins(20, 20, 20, 20);
    
    // 계좌번호 입력 화면 설정 (송금용)
    m_targetAccountWidget = new QWidget(this);
    
    // 상단 헤더
    QWidget *targetHeaderWidget = new QWidget(m_targetAccountWidget);
    targetHeaderWidget->setObjectName("transactionHeader");
    
    m_targetBackButton = new QPushButton("뒤로", m_targetAccountWidget);
    m_targetBackButton->setObjectName("secondaryButton");
    
    QLabel *targetTitleLabel = new QLabel("계좌번호 입력", m_targetAccountWidget);
    targetTitleLabel->setObjectName("pageTitle");
    targetTitleLabel->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout *targetHeaderLayout = new QHBoxLayout(targetHeaderWidget);
    targetHeaderLayout->addWidget(m_targetBackButton);
    targetHeaderLayout->addWidget(targetTitleLabel, 1);
    targetHeaderLayout->addSpacerItem(new QSpacerItem(m_targetBackButton->sizeHint().width(), 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
    targetHeaderLayout->setContentsMargins(20, 20, 20, 20);
    
    // 계좌번호 입력 영역
    QWidget *targetInputCard = new QWidget(m_targetAccountWidget);
    targetInputCard->setObjectName("inputCard");
    
    QLabel *targetInputTitle = new QLabel("송금할 계좌번호", m_targetAccountWidget);
    targetInputTitle->setObjectName("inputTitle");
    
    m_targetAccountEdit = new QLineEdit(m_targetAccountWidget);
    m_targetAccountEdit->setPlaceholderText("계좌번호를 입력하세요");
    m_targetAccountEdit->setObjectName("accountInput");
    
    // 계좌번호 형식 검증 (예: 1234-5678-9012)
    QRegularExpressionValidator *accountValidator = new QRegularExpressionValidator(QRegularExpression("[0-9]{4}-[0-9]{4}-[0-9]{4}"), this);
    m_targetAccountEdit->setValidator(accountValidator);
    
    m_targetAccountErrorLabel = new QLabel("존재하지 않는 계좌번호입니다", m_targetAccountWidget);
    m_targetAccountErrorLabel->setObjectName("errorLabel");
    m_targetAccountErrorLabel->setVisible(false);
    
    QLabel *accountFormatLabel = new QLabel("예시: 1234-5678-9012", m_targetAccountWidget);
    accountFormatLabel->setObjectName("hintLabel");
    
    QVBoxLayout *targetInputCardLayout = new QVBoxLayout(targetInputCard);
    targetInputCardLayout->addWidget(targetInputTitle);
    targetInputCardLayout->addWidget(m_targetAccountEdit);
    targetInputCardLayout->addWidget(m_targetAccountErrorLabel);
    targetInputCardLayout->addWidget(accountFormatLabel);
    targetInputCardLayout->setSpacing(10);
    targetInputCardLayout->setContentsMargins(16, 16, 16, 16);
    
    // 다음 버튼
    m_targetNextButton = new QPushButton("다음", m_targetAccountWidget);
    m_targetNextButton->setObjectName("primaryButton");
    
    QVBoxLayout *targetLayout = new QVBoxLayout(m_targetAccountWidget);
    targetLayout->addWidget(targetHeaderWidget);
    targetLayout->addWidget(targetInputCard);
    targetLayout->addStretch();
    targetLayout->addWidget(m_targetNextButton);
    targetLayout->setContentsMargins(20, 20, 20, 20);
    
    // 스택 위젯에 화면 추가
    m_stackedWidget->addWidget(m_amountInputWidget);
    m_stackedWidget->addWidget(m_targetAccountWidget);
    
    // 메인 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stackedWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // 시그널-슬롯 연결
    connect(m_amountBackButton, &QPushButton::clicked, this, &TransactionWidget::backRequested);
    connect(m_amountNextButton, &QPushButton::clicked, this, &TransactionWidget::onNextButtonClicked);
    connect(m_amountEdit, &QLineEdit::textChanged, this, &TransactionWidget::validateAmount);
    
    connect(m_targetBackButton, &QPushButton::clicked, this, [this]() {
        m_stackedWidget->setCurrentWidget(m_amountInputWidget);
    });
    connect(m_targetNextButton, &QPushButton::clicked, this, &TransactionWidget::onTargetNextButtonClicked);
}

void TransactionWidget::setupForTransaction(const QString &transactionType, const QVariantMap &accountDetails)
{
    m_transactionType = transactionType;
    m_accountBalance = accountDetails["balance"].toDouble();
    
    // 화면 타이틀 설정
    if (transactionType == "deposit") {
        m_transactionTitleLabel->setText("입금 금액 입력");
    } else if (transactionType == "withdraw") {
        m_transactionTitleLabel->setText("출금 금액 입력");
    } else {
        m_transactionTitleLabel->setText("송금 금액 입력");
    }
    
    // 한도 정보 설정
    QLocale locale = QLocale(QLocale::Korean);
    if (transactionType == "deposit") {
        m_limitInfoLabel->setText("0원 초과 ~ 4,200,000,000원 이하로 입력해주세요");
    } else {
        m_limitInfoLabel->setText("0원 초과 ~ " + locale.toString(m_accountBalance, 'f', 0) + "원 이하로 입력해주세요");
    }
    
    // 입력 필드 초기화
    m_amountEdit->clear();
    m_targetAccountEdit->clear();
    m_amountErrorLabel->setVisible(false);
    m_targetAccountErrorLabel->setVisible(false);
    
    // 첫 화면으로 설정
    m_stackedWidget->setCurrentWidget(m_amountInputWidget);
}

void TransactionWidget::showTargetAccountInput()
{
    m_stackedWidget->setCurrentWidget(m_targetAccountWidget);
}

void TransactionWidget::validateAmount()
{
    bool ok;
    double amount = m_amountEdit->text().toDouble(&ok);
    
    if (!ok || amount <= 0) {
        m_amountErrorLabel->setText("0원 초과의 금액을 입력해주세요");
        m_amountErrorLabel->setVisible(true);
        m_amountNextButton->setEnabled(false);
    } else if (amount > 4200000000) {
        m_amountErrorLabel->setText("4,200,000,000원 이하로 입력해주세요");
        m_amountErrorLabel->setVisible(true);
        m_amountNextButton->setEnabled(false);
    } else if ((m_transactionType == "withdraw" || m_transactionType == "transfer") && amount > m_accountBalance) {
        QLocale locale = QLocale(QLocale::Korean);
        m_amountErrorLabel->setText(locale.toString(m_accountBalance, 'f', 0) + "원 이하로 입력해주세요");
        m_amountErrorLabel->setVisible(true);
        m_amountNextButton->setEnabled(false);
    } else {
        m_amountErrorLabel->setVisible(false);
        m_amountNextButton->setEnabled(true);
    }
}

void TransactionWidget::onNextButtonClicked()
{
    bool ok;
    double amount = m_amountEdit->text().toDouble(&ok);
    
    if (ok && amount > 0 && amount <= 4200000000 && 
        (m_transactionType == "deposit" || amount <= m_accountBalance)) {
        emit amountEntered(amount);
    }
}

void TransactionWidget::onTargetNextButtonClicked()
{
    QString targetAccount = m_targetAccountEdit->text();
    
    if (!targetAccount.isEmpty()) {
        emit targetAccountEntered(targetAccount);
    }
}
