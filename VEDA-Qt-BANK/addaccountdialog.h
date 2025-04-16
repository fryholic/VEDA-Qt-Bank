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

    QString accountName() const; // 계좌이름 반환
    QString accountNumber() const;  // 계좌번호 반환

    bool isValidAccountNumber(const QString &accountNumber) const;


private:
    QLineEdit *m_accountNameLineEdit;
    QLineEdit *m_accountNumberLineEdit;

    QPushButton *m_addButton;

};

#endif // ADDACCOUNTDIALOG_H
