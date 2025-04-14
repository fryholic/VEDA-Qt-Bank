#ifndef BANKMODEL_H
#define BANKMODEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariant>
#include <QMap>

class Account {
public:
    QString accountNumber;
    QString accountName;
    double balance;
    
    Account(const QString &number, const QString &name, double initialBalance)
        : accountNumber(number), accountName(name), balance(initialBalance) {}
};

class Transaction {
public:
    QString date;
    QString type;
    double amount;
    QString description;
    
    Transaction(const QString &d, const QString &t, double a, const QString &desc)
        : date(d), type(t), amount(a), description(desc) {}
};

class BankModel : public QObject
{
    Q_OBJECT
    
public:
    explicit BankModel(QObject *parent = nullptr);
    
    QString userName() const { return m_userName; }
    void setUserName(const QString &name);
    
    double totalBalance() const;
    
    QString currentAccountNumber() const { return m_currentAccountNumber; }
    void setCurrentAccountNumber(const QString &accountNumber);
    
    bool login(const QString &username, const QString &password);
    void logout();
    QVariantList getAccounts() const;
    QVariantMap getAccountDetails(const QString &accountNumber) const;
    QVariantList getTransactions(const QString &accountNumber) const;
    bool deposit(const QString &accountNumber, double amount, const QString &verificationCode);
    bool withdraw(const QString &accountNumber, double amount, const QString &verificationCode);
    bool transfer(const QString &fromAccount, const QString &toAccount, double amount, const QString &verificationCode);
    bool verifyAmount(double amount, const QString &type, const QString &accountNumber = QString());
    
signals:
    void userNameChanged();
    void totalBalanceChanged();
    void currentAccountNumberChanged();
    void transactionCompleted(const QString &type, double amount);
    
private:
    QString m_userName;
    QString m_currentAccountNumber;
    QList<Account> m_accounts;
    QMap<QString, QList<Transaction>> m_transactions;
    
    bool isLoggedIn;
    const double MAX_AMOUNT = 4200000000.0; // 42억
};

#endif // BANKMODEL_H
