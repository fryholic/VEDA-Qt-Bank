#include "verificationwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>

VerificationWidget::VerificationWidget(QWidget *parent) : QWidget(parent)
{
    // 상단 헤더
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setObjectName("transactionHeader");
    
    m_backButton = new QPushButton("뒤로", this);
    m_backButton->setObjectName("secondaryButton");
    
    m_titleLabel = new QLabel("인증번호 입력", this);
    m_titleLabel->setObjectName("pageTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->addWidget(m_backButton);
    headerLayout->addWidget(m_titleLabel, 1);
    headerLayout->addSpacerItem(new QSpacerItem(m_backButton->sizeHint().width(), 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
    headerLayout->setContentsMargins(20, 20, 20, 20);
    
    // 인증번호 입력 영역
    QWidget *verificationCard = new QWidget(this);
    verificationCard->setObjectName("inputCard");
    
    QLabel *verificationTitle = new QLabel("ATM 인증번호 5자리를 입력하세요", this);
    verificationTitle->setObjectName("inputTitle");
    
    m_verificationEdit = new QLineEdit(this);
    m_verificationEdit->setPlaceholderText("인증번호");
    m_verificationEdit->setObjectName("verificationInput");
    m_verificationEdit->setMaxLength(5);
    
    // 5자리 숫자만 입력 가능하도록 설정
    QIntValidator *validator = new QIntValidator(0, 99999, this);
    m_verificationEdit->setValidator(validator);
    
    QLabel *hintLabel = new QLabel("* ATM 기기에 뜬 번호를 입력해주세요.", this);
    hintLabel->setObjectName("hintLabel");
    
    QVBoxLayout *verificationCardLayout = new QVBoxLayout(verificationCard);
    verificationCardLayout->addWidget(verificationTitle);
    verificationCardLayout->addWidget(m_verificationEdit);
    verificationCardLayout->addWidget(hintLabel);
    verificationCardLayout->setSpacing(10);
    verificationCardLayout->setContentsMargins(16, 16, 16, 30);
    
    // 다음 버튼
    m_nextButton = new QPushButton("다음", this);
    m_nextButton->setObjectName("primaryButton");
    m_nextButton->setEnabled(false);
    
    // 메인 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(verificationCard);
    mainLayout->addStretch();
    mainLayout->addWidget(m_nextButton);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // 시그널-슬롯 연결
    connect(m_backButton, &QPushButton::clicked, this, &VerificationWidget::backRequested);
    connect(m_nextButton, &QPushButton::clicked, this, &VerificationWidget::onNextButtonClicked);
    connect(m_verificationEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        m_nextButton->setEnabled(text.length() == 5);
    });
}

void VerificationWidget::setupForTransaction(const QString &transactionType)
{
    m_transactionType = transactionType;
    m_verificationEdit->clear();
    m_nextButton->setEnabled(false);
}

void VerificationWidget::onNextButtonClicked()
{
    QString verificationCode = m_verificationEdit->text();
    
    if (verificationCode.length() == 5) {
        emit verificationCompleted(verificationCode);
    }
}
