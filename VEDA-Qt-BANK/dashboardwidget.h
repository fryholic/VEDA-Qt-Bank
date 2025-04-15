#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include "bankmodel.h"

class AccountListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit AccountListItem(const QVariantMap &accountData, QWidget *parent = nullptr);
    
    QString accountNumber() const { return m_accountNumber; }
    
signals:
    void clicked(const QString &accountNumber);
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    
private:
    QString m_accountNumber;
    QLabel *m_accountNameLabel;
    QLabel *m_accountNumberLabel;
    QLabel *m_balanceLabel;
};

class DashboardWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DashboardWidget(BankModel *model, QWidget *parent = nullptr);
    
    void updateUserInfo(const QString &username, double totalBalance);
    void updateAccountList(const QVariantList &accounts);

protected:
    void showEvent(QShowEvent *event) override;
    
signals:
    void logoutRequested();
    void accountSelected(const QString &accountNumber);
    
private:
    QLabel *m_usernameLabel;
    QLabel *m_totalBalanceLabel;
    QListWidget *m_accountListWidget;
    QPushButton *m_logoutButton;

    BankModel *m_bankModel;
};

#endif // DASHBOARDWIDGET_H
