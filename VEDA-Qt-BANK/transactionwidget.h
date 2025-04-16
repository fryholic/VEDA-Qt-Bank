#ifndef TRANSACTIONWIDGET_H
#define TRANSACTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QValidator>
#include "bankmodel.h"

class TransactionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TransactionWidget(BankModel *bankModel, QWidget *parent = nullptr);
    
    void setupForTransaction(const QString &transactionType, const QVariantMap &accountDetails);
    void showTargetAccountInput();
    
signals:
    void backRequested();
    void amountEntered(double amount);
    void targetAccountEntered(const QString &targetAccount);
    
private slots:
    void onNextButtonClicked();
    void onTargetNextButtonClicked();
    
private:
    BankModel *m_bankModel;

    QString m_transactionType;
    double m_accountBalance;  
    
    QStackedWidget *m_stackedWidget;
    
    // 금액 입력 화면
    QWidget *m_amountInputWidget;
    QLabel *m_transactionTitleLabel;
    QLineEdit *m_amountEdit;
    QLabel *m_amountErrorLabel;
    QLabel *m_limitInfoLabel;
    QPushButton *m_amountBackButton;
    QPushButton *m_amountNextButton;
    
    // 계좌번호 입력 화면 (송금용)
    QWidget *m_targetAccountWidget;
    QLineEdit *m_targetAccountEdit;
    QLabel *m_targetAccountErrorLabel;
    QPushButton *m_targetBackButton;
    QPushButton *m_targetNextButton;
    
    void validateAmount();
};

class QInt64Validator : public QValidator {
public:
    QInt64Validator(qint64 min, qint64 max, QObject *parent = nullptr)
        : QValidator(parent), m_min(min), m_max(max) {}

    State validate(QString &input, int &pos) const override {
        if (input.isEmpty()) return Intermediate;

        // 과학적 표기법 및 소수점 차단
        QRegularExpression regex("^-?\\d{0,10}$"); // 최대 10자리
        if (!regex.match(input).hasMatch()) {
            return Invalid;
        }

        bool ok;
        qint64 value = input.toLongLong(&ok);
        if (!ok) return Invalid;

        if (value >= m_min && value <= m_max) return Acceptable;
        return Intermediate;
    }

private:
    qint64 m_min, m_max;
};

#endif // TRANSACTIONWIDGET_H
