#include "completionwidget.h"
#include "checkanimationwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLocale>
#include <QStyle>
#include <QTimer>

CompletionWidget::CompletionWidget(QWidget *parent) : QWidget(parent)
{
    // 완료 아이콘 (애니메이션 위젯으로 변경)
    m_checkAnimation = new CheckAnimationWidget(this);
    
    // 완료 텍스트
    m_completeTextLabel = new QLabel(this);
    m_completeTextLabel->setObjectName("completeText");
    m_completeTextLabel->setAlignment(Qt::AlignCenter);
    
    // 금액 텍스트
    m_amountLabel = new QLabel(this);
    m_amountLabel->setObjectName("completeAmount");
    m_amountLabel->setAlignment(Qt::AlignCenter);
    
    // 계좌 관리로 돌아가기 버튼
    QPushButton *backToAccountButton = new QPushButton("계좌 관리로 돌아가기", this);
    backToAccountButton->setObjectName("primaryButton");
    
    // 메인 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    
    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(30);
    centerLayout->addWidget(m_checkAnimation, 0, Qt::AlignCenter);
    centerLayout->addWidget(m_completeTextLabel);
    centerLayout->addWidget(m_amountLabel);
    centerLayout->addWidget(backToAccountButton);
    
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->addStretch();
    horizontalLayout->addLayout(centerLayout);
    horizontalLayout->addStretch();
    
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch();
    
    // 시그널-슬롯 연결
    connect(backToAccountButton, &QPushButton::clicked, this, &CompletionWidget::backToAccountRequested);
}

void CompletionWidget::setupForTransaction(const QString &transactionType, double amount)
{
    // 거래 유형에 따른 텍스트 설정
    if (transactionType == "deposit") {
        m_completeTextLabel->setText("입금이 완료되었습니다");
    } else if (transactionType == "withdraw") {
        m_completeTextLabel->setText("출금이 완료되었습니다");
    } else {
        m_completeTextLabel->setText("송금이 완료되었습니다");
    }
    
    // 애니메이션 위젯 설정
    m_checkAnimation->setTransactionType(transactionType);
    
    // 금액 표시
    QLocale locale = QLocale(QLocale::Korean);
    m_amountLabel->setText(locale.toString(amount, 'f', 0) + " 원");
    
    // 애니메이션 시작 (약간의 지연 후)
    QTimer::singleShot(100, m_checkAnimation, &CheckAnimationWidget::startAnimation);
}
