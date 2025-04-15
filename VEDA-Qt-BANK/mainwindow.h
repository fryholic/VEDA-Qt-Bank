#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "bankmodel.h"
#include "loginwidget.h"
#include "dashboardwidget.h"
#include "accountwidget.h"
#include "transactionwidget.h"
#include "verificationwidget.h"
#include "completionwidget.h"
#include "registerdialog.h"
#include <QStack>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    // 화면 전환 메서드
    void showLoginScreen();
    void showDashboardScreen();
    void showAccountScreen(const QString &accountNumber);
    void showTransactionScreen(const QString &accountNumber, const QString &transactionType);
    void showVerificationScreen();
    void showCompletionScreen();
    
private slots:
    void onLoginSuccess(const QString &username);
    void onLogout();
    void onAccountSelected(const QString &accountNumber);
    void onTransactionRequested(const QString &accountNumber, const QString &transactionType);
    void onTransactionAmountEntered(double amount);
    void onTransactionTargetEntered(const QString &targetAccount);
    void onVerificationCompleted(const QString &verificationCode);
    void onTransactionCompleted();
    void onBackRequested();
    
private:
    QStackedWidget *m_stackedWidget;
    BankModel *m_bankModel;
    
    // 화면 위젯
    LoginWidget *m_loginWidget;
    DashboardWidget *m_dashboardWidget;
    AccountWidget *m_accountWidget;
    TransactionWidget *m_transactionWidget;
    VerificationWidget *m_verificationWidget;
    CompletionWidget *m_completionWidget;
    
    // 현재 거래 정보
    QString m_currentAccountNumber;
    QString m_currentTransactionType;
    double m_currentTransactionAmount;
    QString m_currentTargetAccount;
    
    // 이전 화면 스택
    QStack<int> m_screenStack;
};

#endif // MAINWINDOW_H
