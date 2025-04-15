#ifndef TRANSACTIONWIDGET_H
#define TRANSACTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>

class TransactionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TransactionWidget(QWidget *parent = nullptr);
    
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

#endif // TRANSACTIONWIDGET_H
