#include "addaccountdialog.h"
#include "dashboardwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleValidator>


AddAccountDialog::AddAccountDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("새 계좌 생성");

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("계좌 이름");

    m_balanceEdit = new QLineEdit(this);
    m_balanceEdit->setPlaceholderText("초기 잔액");
    m_balanceEdit->setValidator(new QDoubleValidator(0, 1e9, 2, this));

    m_addButton = new QPushButton("계좌 생성", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("계좌이름"));
    layout->addWidget(m_nameEdit);
    layout->addWidget(new QLabel("초기잔액"));
    layout->addWidget(m_balanceEdit);
    layout->addWidget(m_addButton);

    connect(m_addButton, &QPushButton::clicked, this, &QDialog::accept);
}

QString AddAccountDialog::accountName() const {
    return m_nameEdit->text();
}

double AddAccountDialog::initialBalance() const {
    return m_balanceEdit->text().toDouble();
}
