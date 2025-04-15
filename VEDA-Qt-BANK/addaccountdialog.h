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

    QString accountName() const;
    double initialBalance() const;

private:
    QLineEdit *m_nameEdit;
    QLineEdit *m_balanceEdit;
    QPushButton *m_addButton;
};

#endif // ADDACCOUNTDIALOG_H
