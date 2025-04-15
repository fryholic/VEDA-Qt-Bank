#include "completionwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLocale>
#include <QStyle>

CompletionWidget::CompletionWidget(QWidget *parent) : QWidget(parent)
{
    m_movie = nullptr;

    // 완료 아이콘
    m_iconLabel = new QLabel(this);
    m_iconLabel->setObjectName("completeIcon");
    m_iconLabel->setFixedSize(120, 120);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    
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
    centerLayout->addWidget(m_iconLabel, 0, Qt::AlignCenter);
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
    if(m_movie){
        m_movie->deleteLater();
        m_movie = nullptr;
    }

    QString gifPath = ":/images/check.gif";

    // 거래 유형에 따른 텍스트 설정
    if (transactionType == "deposit") {
        m_completeTextLabel->setText("입금이 완료되었습니다");
        m_iconLabel->setProperty("transactionType", "deposit");
    } else if (transactionType == "withdraw") {
        m_completeTextLabel->setText("출금이 완료되었습니다");
        m_iconLabel->setProperty("transactionType", "withdraw");
    } else {
        m_completeTextLabel->setText("송금이 완료되었습니다");
        m_iconLabel->setProperty("transactionType", "transfer");
    }
    
    m_movie = new QMovie(gifPath, QByteArray(), this);
    m_iconLabel->setMovie(m_movie);
    m_movie->start();


    // 금액 표시
    QLocale locale = QLocale(QLocale::Korean);
    m_amountLabel->setText(locale.toString(amount, 'f', 0) + " 원");
    
    // 스타일 업데이트
    m_iconLabel->style()->unpolish(m_iconLabel);
    m_iconLabel->style()->polish(m_iconLabel);
}
