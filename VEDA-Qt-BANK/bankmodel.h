#ifndef BANKMODEL_H
#define BANKMODEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariant>
#include <QMap>
#include <QSqlDatabase>

class Account {
public:
    QString accountNumber;
    QString accountName;
    qint64 balance;
    
    Account(const QString &number, const QString &name, qint64 initialBalance)
        : accountNumber(number), accountName(name), balance(initialBalance) {}
};

class Transaction {
public:
    QString date;
    QString type;
    qint64 amount;
    QString description;
    
    Transaction(const QString &d, const QString &t, qint64 a, const QString &desc)
        : date(d), type(t), amount(a), description(desc) {}
};

class BankModel : public QObject
{
    Q_OBJECT
    
public:
    explicit BankModel(QObject *parent = nullptr);
    ~BankModel();
    
    QString userName() const { return m_userName; }
    void setUserName(const QString &name);
    
    qint64 totalBalance() const;
    
    QString currentAccountNumber() const { return m_currentAccountNumber; }
    void setCurrentAccountNumber(const QString &accountNumber);
    
    void logout();
    QVariantList getAccounts() const;
    QVariantMap getAccountDetails(const QString &accountNumber) const;
    QVariantList getTransactions(const QString &accountNumber) const;
    bool deposit(const QString &accountNumber, qint64 amount, const QString &verificationCode);
    bool withdraw(const QString &accountNumber, qint64 amount, const QString &verificationCode);
    bool transfer(const QString &fromAccount, const QString &toAccount, qint64 amount, const QString &verificationCode);
    bool verifyAmount(qint64 amount, const QString &type, const QString &accountNumber = QString());

    // bankmodel.h 안에 public 함수 추가
    bool registerUser(const QString &username, const QString &password); // 회원가입
    bool login(const QString &username, const QString &password); // 로그인

    bool createAccount(const QString &accountName, const QString &accountNumber);

signals:
    void userNameChanged();
    void totalBalanceChanged();
    void currentAccountNumberChanged();
    void transactionCompleted(const QString &type, qint64 amount);
    
private:
    QString m_userName;
    QString m_currentAccountNumber;
    QList<Account> m_accounts;
    QMap<QString, QList<Transaction>> m_transactions;
    QSqlDatabase db;

    void initializeDatabase();
    
    bool isLoggedIn;
    const qint64 MAX_AMOUNT = 4200000000; // 42억

    // private 함수 안에 추가
    void createUserTable(); // 사용자 테이블 생성
};

#endif // BANKMODEL_H
