#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);
/*
    QString accountName() const;
    double initialBalance() const;
*/
    QString bankName() const;       // 은행명 반환
    QString accountNumber() const;  // 계좌번호 반환

    bool isValidAccountNumber(const QString &accountNumber) const;


private:
    /*
    QLineEdit *m_nameEdit;
    QLineEdit *m_balanceEdit;
    */
    QLineEdit *m_bankNameLineEdit;
    QLineEdit *m_accountNumberLineEdit;

    QPushButton *m_addButton;

};

#endif // ADDACCOUNTDIALOG_H
