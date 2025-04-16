#include "addaccountdialog.h"
#include "dashboardwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleValidator>


AddAccountDialog::AddAccountDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("새 계좌 등록");

    m_accountNameLineEdit = new QLineEdit(this);
    m_accountNameLineEdit->setPlaceholderText("계좌 이름을 입력해주세요.");

    m_accountNumberLineEdit = new QLineEdit(this);
    m_accountNumberLineEdit->setPlaceholderText("계좌 번호를 입력해주세요.");
    m_accountNumberLineEdit->setValidator(new QDoubleValidator(0, 1e9, 2, this));

    m_addButton = new QPushButton("계좌 연결", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("계좌 이름"));
    layout->addWidget(m_accountNameLineEdit);
    layout->addWidget(new QLabel("계좌 번호"));
    layout->addWidget(m_accountNumberLineEdit);
    layout->addWidget(m_addButton);

    connect(m_addButton, &QPushButton::clicked, this, &QDialog::accept);
}

QString AddAccountDialog::accountName() const {
    return m_accountNameLineEdit->text();
}

QString AddAccountDialog::accountNumber() const {
    return m_accountNumberLineEdit->text();
}

bool AddAccountDialog::isValidAccountNumber(const QString &accountNumber) const {
    QRegularExpression re("^\\d{12}$"); // 정확히 12자리 숫자
    return re.match(accountNumber).hasMatch();
}
