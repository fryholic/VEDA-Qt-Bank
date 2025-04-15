#include "bankmodel.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>

BankModel::BankModel(QObject *parent) : QObject(parent), isLoggedIn(false)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bank.db"); // 데이터베이스 파일 이름
    if (!db.open()) {
        qDebug() << "Error: Unable to open database:" << db.lastError().text();
    } else {
        initializeDatabase(); // 테이블 생성
    }
}

BankModel::~BankModel()
{
    if (db.isOpen()) {
        db.close();
    }
}

void BankModel::setUserName(const QString &name)
{
    if (m_userName != name) {
        m_userName = name;
        emit userNameChanged();
    }
}

double BankModel::totalBalance() const
{
    QSqlQuery query("SELECT SUM(balance) as total FROM accounts");
    if (query.next()) {
        return query.value("total").toDouble();
    }
    return 0.0;
}

void BankModel::setCurrentAccountNumber(const QString &accountNumber)
{
    if (m_currentAccountNumber != accountNumber) {
        m_currentAccountNumber = accountNumber;
        emit currentAccountNumberChanged();
    }
}

bool BankModel::login(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (query.exec() && query.next()) {
        setUserName(username);
        isLoggedIn = true;
        return true;
    }
    return false;
}

void BankModel::logout()
{
    isLoggedIn = false;
    m_userName = "";
    emit userNameChanged();
}

QVariantList BankModel::getAccounts() const
{
    // QVariantList result;
    // for (const Account &account : m_accounts) {
    //     QVariantMap accountMap;
    //     accountMap["accountNumber"] = account.accountNumber;
    //     accountMap["accountName"] = account.accountName;
    //     accountMap["balance"] = account.balance;
    //     result.append(accountMap);
    // }
    // return result;

/*
    QVariantList result;
    QSqlQuery query("SELECT * FROM accounts");
    while (query.next()) {
        QVariantMap accountMap;
        accountMap["accountNumber"] = query.value("accountNumber").toString();
        accountMap["accountName"] = query.value("accountName").toString();
        accountMap["balance"] = query.value("balance").toDouble();
        result.append(accountMap);
    }
    return result;
*/
    QVariantList result;
    QSqlQuery query;
    query.prepare("SELECT * FROM accounts WHERE username = :username");
    query.bindValue(":username", m_userName);
    query.exec();

    while (query.next()) {
        QVariantMap accountMap;
        accountMap["accountNumber"] = query.value("accountNumber").toString();
        accountMap["accountName"] = query.value("accountName").toString();
        accountMap["balance"] = query.value("balance").toDouble();
        result.append(accountMap);
    }

    return result;
}

QVariantMap BankModel::getAccountDetails(const QString &accountNumber) const
{
    QVariantMap result;
    QSqlQuery query;
    query.prepare("SELECT * FROM accounts WHERE accountNumber = :accountNumber");
    query.bindValue(":accountNumber", accountNumber);
    if (query.exec() && query.next()) {
        result["accountNumber"] = query.value("accountNumber").toString();
        result["accountName"] = query.value("accountName").toString();
        result["balance"] = query.value("balance").toDouble();
    }
    return result;
}

QVariantList BankModel::getTransactions(const QString &accountNumber) const
{
    QVariantList result;
    QSqlQuery query;
    query.prepare("SELECT * FROM transactions WHERE accountNumber = :accountNumber");
    query.bindValue(":accountNumber", accountNumber);
    if (query.exec()) {
        while (query.next()) {
            QVariantMap transactionMap;
            transactionMap["date"] = query.value("date").toString();
            transactionMap["type"] = query.value("type").toString();
            transactionMap["amount"] = query.value("amount").toDouble();
            transactionMap["description"] = query.value("description").toString();
            result.append(transactionMap);
        }
    }
    return result;
}

bool BankModel::deposit(const QString &accountNumber, double amount, const QString &verificationCode)
{
    // // 검증 코드는 5자리 숫자여야 함
    // if (verificationCode.length() != 5 || !verificationCode.toInt()) {
    //     return false;
    // }
    
    // if (!verifyAmount(amount, "deposit")) {
    //     return false;
    // }
    
    // for (int i = 0; i < m_accounts.size(); ++i) {
    //     if (m_accounts[i].accountNumber == accountNumber) {
    //         m_accounts[i].balance += amount;
            
    //         // 거래 내역 추가
    //         QDateTime now = QDateTime::currentDateTime();
    //         Transaction transaction(now.toString("yyyy-MM-dd"), "입금", amount, "입금");
    //         m_transactions[accountNumber].append(transaction);
            
    //         emit totalBalanceChanged();
    //         emit transactionCompleted("deposit", amount);
    //         return true;
    //     }
    // }
    
    // return false;

    if (verificationCode.length() != 5 || !verificationCode.toInt()) {
        return false;
    }

    if (!verifyAmount(amount, "deposit")) {
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE accounts SET balance = balance + :amount WHERE accountNumber = :accountNumber");
    query.bindValue(":amount", amount);
    query.bindValue(":accountNumber", accountNumber);
    if (!query.exec()) {
        qDebug() << "Deposit error:" << query.lastError().text();
        return false;
    }

    QDateTime now = QDateTime::currentDateTime();
    query.prepare("INSERT INTO transactions (accountNumber, date, type, amount, description) "
                  "VALUES (:accountNumber, :date, :type, :amount, :description)");
    query.bindValue(":accountNumber", accountNumber);
    query.bindValue(":date", now.toString("yyyy-MM-dd"));
    query.bindValue(":type", "입금");
    query.bindValue(":amount", amount);
    query.bindValue(":description", "입금");
    if (!query.exec()) {
        qDebug() << "Transaction insert error:" << query.lastError().text();
        return false;
    }

    emit totalBalanceChanged();
    emit transactionCompleted("deposit", amount);
    return true;
}

bool BankModel::withdraw(const QString &accountNumber, double amount, const QString &verificationCode)
{
    if (verificationCode.length() != 5 || !verificationCode.toInt()) {
        return false;
    }

    if (!verifyAmount(amount, "withdraw", accountNumber)) {
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE accounts SET balance = balance - :amount WHERE accountNumber = :accountNumber");
    query.bindValue(":amount", amount);
    query.bindValue(":accountNumber", accountNumber);
    if (!query.exec()) {
        qDebug() << "Withdraw error:" << query.lastError().text();
        return false;
    }

    QDateTime now = QDateTime::currentDateTime();
    query.prepare("INSERT INTO transactions (accountNumber, date, type, amount, description) "
                  "VALUES (:accountNumber, :date, :type, :amount, :description)");
    query.bindValue(":accountNumber", accountNumber);
    query.bindValue(":date", now.toString("yyyy-MM-dd"));
    query.bindValue(":type", "출금");
    query.bindValue(":amount", amount);
    query.bindValue(":description", "출금");
    if (!query.exec()) {
        qDebug() << "Transaction insert error:" << query.lastError().text();
        return false;
    }

    emit totalBalanceChanged();
    emit transactionCompleted("withdraw", amount);
    return true;
}

bool BankModel::transfer(const QString &fromAccount, const QString &toAccount, double amount, const QString &verificationCode)
{
    if (verificationCode.length() != 5 || !verificationCode.toInt()) {
        return false;
    }

    if (!verifyAmount(amount, "transfer", fromAccount)) {
        return false;
    }

    QSqlQuery query;
    // 출금
    query.prepare("UPDATE accounts SET balance = balance - :amount WHERE accountNumber = :accountNumber");
    query.bindValue(":amount", amount);
    query.bindValue(":accountNumber", fromAccount);
    if (!query.exec()) {
        qDebug() << "Transfer (withdraw) error:" << query.lastError().text();
        return false;
    }

    // 입금
    query.prepare("UPDATE accounts SET balance = balance + :amount WHERE accountNumber = :accountNumber");
    query.bindValue(":amount", amount);
    query.bindValue(":accountNumber", toAccount);
    if (!query.exec()) {
        qDebug() << "Transfer (deposit) error:" << query.lastError().text();
        return false;
    }

    QDateTime now = QDateTime::currentDateTime();
    // 출금 거래 내역
    query.prepare("INSERT INTO transactions (accountNumber, date, type, amount, description) "
                  "VALUES (:accountNumber, :date, :type, :amount, :description)");
    query.bindValue(":accountNumber", fromAccount);
    query.bindValue(":date", now.toString("yyyy-MM-dd"));
    query.bindValue(":type", "송금");
    query.bindValue(":amount", amount);
    query.bindValue(":description", "송금: " + toAccount);
    if (!query.exec()) {
        qDebug() << "Transfer (from) transaction error:" << query.lastError().text();
        return false;
    }

    // 입금 거래 내역
    query.prepare("INSERT INTO transactions (accountNumber, date, type, amount, description) "
                  "VALUES (:accountNumber, :date, :type, :amount, :description)");
    query.bindValue(":accountNumber", toAccount);
    query.bindValue(":date", now.toString("yyyy-MM-dd"));
    query.bindValue(":type", "입금");
    query.bindValue(":amount", amount);
    query.bindValue(":description", "송금: " + fromAccount);
    if (!query.exec()) {
        qDebug() << "Transfer (to) transaction error:" << query.lastError().text();
        return false;
    }

    emit totalBalanceChanged();
    emit transactionCompleted("transfer", amount);
    return true;
}

bool BankModel::verifyAmount(double amount, const QString &type, const QString &accountNumber)
{
    if (amount <= 0 || amount > MAX_AMOUNT) {
        return false;
    }

    if (type == "withdraw" || type == "transfer") {
        QSqlQuery query;
        query.prepare("SELECT balance FROM accounts WHERE accountNumber = :accountNumber");
        query.bindValue(":accountNumber", accountNumber);
        if (query.exec() && query.next()) {
            double balance = query.value("balance").toDouble();
            if (balance < amount) {
                return false;
            }
        } else {
            return false; // 계좌가 없거나 쿼리 실패
        }
    }

    return true;
}

void BankModel::initializeDatabase()
{
    QSqlQuery query;

    // bankmodel.cpp
    query.exec("CREATE TABLE IF NOT EXISTS accounts ("
               "accountNumber TEXT PRIMARY KEY, "
               "accountName TEXT, "
               "balance REAL, "
               "username TEXT)");

/*
    // 계좌 테이블 생성
    query.exec("CREATE TABLE IF NOT EXISTS accounts ("
               "accountNumber TEXT PRIMARY KEY, "
               "accountName TEXT, "
               "balance REAL)");
*/
    if (query.lastError().isValid()) {
        qDebug() << "Error creating accounts table:" << query.lastError().text();
    }


    // 거래 내역 테이블 생성
    query.exec("CREATE TABLE IF NOT EXISTS transactions ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "accountNumber TEXT, "
               "date TEXT, "
               "type TEXT, "
               "amount REAL, "
               "description TEXT)");
    if (query.lastError().isValid()) {
        qDebug() << "Error creating transactions table:" << query.lastError().text();
    }
/*
    // 초기 데이터 삽입 (필요 시)
    query.exec("INSERT OR IGNORE INTO accounts (accountNumber, accountName, balance) VALUES "
               "('1234-5678-9012', '일반 계좌', 1000000), "
               "('9876-5432-1098', '저축 계좌', 5000000), "
               "('5555-6666-7777', '투자 계좌', 10000000)");

*/
    createUserTable();

}

void BankModel::createUserTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "username TEXT PRIMARY KEY, "
               "password TEXT)");
    if (query.lastError().isValid()) {
        qDebug() << "Error creating users table:" << query.lastError().text();
    }
}

bool BankModel::registerUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password); // 실제 서비스에선 해시 필요!
    if (!query.exec()) {
        qDebug() << "Register failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BankModel::createAccount(const QString &accountName, double initialBalance)
{
    QString accountNumber = QString::number(QRandomGenerator::global()->bounded(1000000000, 9999999999));
    QString username = m_userName; // 현재 로그인한 사용자 이름

    QSqlQuery query;
    query.prepare("INSERT INTO accounts (accountNumber, accountName, balance, username) "
                  "VALUES (:accountNumber, :accountName, :balance, :username)");
    query.bindValue(":accountNumber", accountNumber);
    query.bindValue(":accountName", accountName);
    query.bindValue(":balance", initialBalance);
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "계좌 생성 실패:" << query.lastError().text();
        return false;
    }

    return true;
}
