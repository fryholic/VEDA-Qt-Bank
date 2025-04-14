#include "bankmodel.h"
#include <QDateTime>

BankModel::BankModel(QObject *parent) : QObject(parent), isLoggedIn(false)
{
    // 테스트 데이터 초기화
    m_accounts.append(Account("1234-5678-9012", "일반 계좌", 1000000));
    m_accounts.append(Account("9876-5432-1098", "저축 계좌", 5000000));
    m_accounts.append(Account("5555-6666-7777", "투자 계좌", 10000000));

    QList<Transaction> transactions1;
    transactions1.append(Transaction("2023-04-01", "입금", 500000, "급여"));
    transactions1.append(Transaction("2023-04-05", "출금", 100000, "생활비"));
    transactions1.append(Transaction("2023-04-10", "송금", 50000, "친구에게 송금"));
    m_transactions["1234-5678-9012"] = transactions1;

    QList<Transaction> transactions2;
    transactions2.append(Transaction("2023-03-15", "입금", 1000000, "보너스"));
    transactions2.append(Transaction("2023-03-20", "출금", 200000, "가전제품 구매"));
    transactions2.append(Transaction("2023-03-25", "입금", 300000, "환불"));
    m_transactions["9876-5432-1098"] = transactions2;

    QList<Transaction> transactions3;
    transactions3.append(Transaction("2023-02-10", "입금", 5000000, "투자금"));
    transactions3.append(Transaction("2023-02-15", "입금", 5000000, "투자금"));
    m_transactions["5555-6666-7777"] = transactions3;
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
    double total = 0.0;
    for (const Account &account : m_accounts) {
        total += account.balance;
    }
    return total;
}

void BankModel::setCurrentAccountNumber(const QString &accountNumber)
{
    if (m_currentAccountNumber != accountNumber) {
        m_currentAccountNumber = accountNumber;
        emit currentAccountNumberChanged();
    }
}

bool BankModel::login(const QString &username, const QString &password)
{
    // 간단한 인증 로직 (실제로는 더 복잡한 인증이 필요)
    if (password == "2") {
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
    QVariantList result;
    for (const Account &account : m_accounts) {
        QVariantMap accountMap;
        accountMap["accountNumber"] = account.accountNumber;
        accountMap["accountName"] = account.accountName;
        accountMap["balance"] = account.balance;
        result.append(accountMap);
    }
    return result;
}

QVariantMap BankModel::getAccountDetails(const QString &accountNumber) const
{
    QVariantMap result;
    for (const Account &account : m_accounts) {
        if (account.accountNumber == accountNumber) {
            result["accountNumber"] = account.accountNumber;
            result["accountName"] = account.accountName;
            result["balance"] = account.balance;
            break;
        }
    }
    return result;
}

QVariantList BankModel::getTransactions(const QString &accountNumber) const
{
    QVariantList result;
    if (m_transactions.contains(accountNumber)) {
        for (const Transaction &transaction : m_transactions[accountNumber]) {
            QVariantMap transactionMap;
            transactionMap["date"] = transaction.date;
            transactionMap["type"] = transaction.type;
            transactionMap["amount"] = transaction.amount;
            transactionMap["description"] = transaction.description;
            result.append(transactionMap);
        }
    }
    return result;
}

bool BankModel::deposit(const QString &accountNumber, double amount, const QString &verificationCode)
{
    // 검증 코드는 5자리 숫자여야 함
    if (verificationCode.length() != 5 || !verificationCode.toInt()) {
        return false;
    }

    if (!verifyAmount(amount, "deposit")) {
        return false;
    }

    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].accountNumber == accountNumber) {
            m_accounts[i].balance += amount;

            // 거래 내역 추가
            QDateTime now = QDateTime::currentDateTime();
            Transaction transaction(now.toString("yyyy-MM-dd"), "입금", amount, "입금");
            m_transactions[accountNumber].append(transaction);

            emit totalBalanceChanged();
            emit transactionCompleted("deposit", amount);
            return true;
        }
    }

    return false;
}

bool BankModel::withdraw(const QString &accountNumber, double amount, const QString &verificationCode)
{
    // 검증 코드는 5자리 숫자여야 함
    if (verificationCode.length() != 5 || !verificationCode.toInt()) {
        return false;
    }

    if (!verifyAmount(amount, "withdraw", accountNumber)) {
        return false;
    }

    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].accountNumber == accountNumber) {
            if (m_accounts[i].balance < amount) {
                return false; // 잔액 부족
            }

            m_accounts[i].balance -= amount;

            // 거래 내역 추가
            QDateTime now = QDateTime::currentDateTime();
            Transaction transaction(now.toString("yyyy-MM-dd"), "출금", amount, "출금");
            m_transactions[accountNumber].append(transaction);

            emit totalBalanceChanged();
            emit transactionCompleted("withdraw", amount);
            return true;
        }
    }

    return false;
}

bool BankModel::transfer(const QString &fromAccount, const QString &toAccount, double amount, const QString &verificationCode)
{
    // 검증 코드는 5자리 숫자여야 함
    if (verificationCode.length() != 5 || !verificationCode.toInt()) {
        return false;
    }

    if (!verifyAmount(amount, "transfer", fromAccount)) {
        return false;
    }

    int fromIndex = -1;
    int toIndex = -1;

    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].accountNumber == fromAccount) {
            fromIndex = i;
        }
        if (m_accounts[i].accountNumber == toAccount) {
            toIndex = i;
        }
    }

    if (fromIndex == -1 || toIndex == -1) {
        return false; // 계좌 없음
    }

    if (m_accounts[fromIndex].balance < amount) {
        return false; // 잔액 부족
    }

    m_accounts[fromIndex].balance -= amount;
    m_accounts[toIndex].balance += amount;

    // 거래 내역 추가
    QDateTime now = QDateTime::currentDateTime();
    Transaction transactionFrom(now.toString("yyyy-MM-dd"), "송금", amount, "송금: " + toAccount);
    m_transactions[fromAccount].append(transactionFrom);

    Transaction transactionTo(now.toString("yyyy-MM-dd"), "입금", amount, "송금: " + fromAccount);
    m_transactions[toAccount].append(transactionTo);

    emit totalBalanceChanged();
    emit transactionCompleted("transfer", amount);
    return true;
}

bool BankModel::verifyAmount(double amount, const QString &type, const QString &accountNumber)
{
    if (amount <= 0) {
        return false; // 0원 이하 금액 불가
    }

    if (amount > MAX_AMOUNT) {
        return false; // 42억 초과 금액 불가
    }

    if (type == "withdraw" || type == "transfer") {
        // 출금이나 송금의 경우 계좌 잔액 확인
        for (const Account &account : m_accounts) {
            if (account.accountNumber == accountNumber) {
                if (account.balance < amount) {
                    return false; // 잔액 부족
                }
                break;
            }
        }
    }

    return true;
}
