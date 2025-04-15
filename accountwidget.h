#ifndef ACCOUNTWIDGET_H
#define ACCOUNTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>

class TransactionListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit TransactionListItem(const QVariantMap &transactionData, QWidget *parent = nullptr);
    
private:
    QLabel *m_dateLabel;
    QLabel *m_descriptionLabel;
    QLabel *m_amountLabel;
};

class AccountWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AccountWidget(QWidget *parent = nullptr);
    
    void updateAccountDetails(const QVariantMap &accountDetails);
    void updateTransactionList(const QVariantList &transactions);
    
signals:
    void backRequested();
    void transactionRequested(const QString &accountNumber, const QString &transactionType);
    
private slots:
    void onDepositClicked();
    void onWithdrawClicked();
    void onTransferClicked();
    
private:
    QString m_accountNumber;
    QLabel *m_accountNameLabel;
    QLabel *m_accountNumberLabel;
    QLabel *m_balanceLabel;
    QListWidget *m_transactionListWidget;
    QPushButton *m_depositButton;
    QPushButton *m_withdrawButton;
    QPushButton *m_transferButton;
    QPushButton *m_backButton;
};

#endif // ACCOUNTWIDGET_H
