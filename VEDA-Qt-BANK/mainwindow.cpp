#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 기본 창 설정
    setWindowTitle("VEDA 은행");
    resize(800, 600);
    
    // 모델 초기화
    m_bankModel = new BankModel(this);
    
    // 스택 위젯 설정
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // 화면 위젯 초기화
    m_loginWidget = new LoginWidget(this);
    m_dashboardWidget = new DashboardWidget(m_bankModel, this);
    m_accountWidget = new AccountWidget(this);
    m_transactionWidget = new TransactionWidget(this);
    m_verificationWidget = new VerificationWidget(this);
    m_completionWidget = new CompletionWidget(this);
    
    // 스택 위젯에 추가
    m_stackedWidget->addWidget(m_loginWidget);
    m_stackedWidget->addWidget(m_dashboardWidget);
    m_stackedWidget->addWidget(m_accountWidget);
    m_stackedWidget->addWidget(m_transactionWidget);
    m_stackedWidget->addWidget(m_verificationWidget);
    m_stackedWidget->addWidget(m_completionWidget);
    
    // 시그널-슬롯 연결
    connect(m_loginWidget, &LoginWidget::loginRequested, this, [this](const QString &username, const QString &password) {
        if (m_bankModel->login(username, password)) {
            onLoginSuccess(username);
        } else {
            QMessageBox::warning(this, "로그인 오류", "아이디 또는 비밀번호가 올바르지 않습니다.");
        }
    });
    
    connect(m_dashboardWidget, &DashboardWidget::logoutRequested, this, &MainWindow::onLogout);
    connect(m_dashboardWidget, &DashboardWidget::accountSelected, this, &MainWindow::onAccountSelected);
    
    connect(m_accountWidget, &AccountWidget::backRequested, this, &MainWindow::onBackRequested);
    connect(m_accountWidget, &AccountWidget::transactionRequested, this, &MainWindow::onTransactionRequested);
    
    connect(m_transactionWidget, &TransactionWidget::backRequested, this, &MainWindow::onBackRequested);
    connect(m_transactionWidget, &TransactionWidget::amountEntered, this, &MainWindow::onTransactionAmountEntered);
    connect(m_transactionWidget, &TransactionWidget::targetAccountEntered, this, &MainWindow::onTransactionTargetEntered);
    
    connect(m_verificationWidget, &VerificationWidget::backRequested, this, &MainWindow::onBackRequested);
    connect(m_verificationWidget, &VerificationWidget::verificationCompleted, this, &MainWindow::onVerificationCompleted);
    
    connect(m_completionWidget, &CompletionWidget::backToAccountRequested, this, [this]() {
        showAccountScreen(m_currentAccountNumber);
    });
    
    connect(m_loginWidget, &LoginWidget::registerRequested, this, [&]() {
        RegisterDialog dialog;
        connect(&dialog, &RegisterDialog::registerUser, this, [&](const QString &id, const QString &pw) {
            if (m_bankModel->registerUser(id, pw)) {
                QMessageBox::information(nullptr, "회원가입", "회원가입이 완료되었습니다.");
            } else {
                QMessageBox::warning(nullptr, "회원가입", "이미 존재하는 아이디입니다.");
            }
        });
        dialog.exec();
    });

    // 초기 화면 표시
    showLoginScreen();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLoginScreen()
{
    m_screenStack.clear();
    m_stackedWidget->setCurrentWidget(m_loginWidget);
}

void MainWindow::showDashboardScreen()
{
    m_screenStack.push(m_stackedWidget->currentIndex());
    m_stackedWidget->setCurrentWidget(m_dashboardWidget);
}

void MainWindow::showAccountScreen(const QString &accountNumber)
{
    // m_screenStack.clear(); // 이전 스택 비우기
    m_screenStack.push(m_stackedWidget->indexOf(m_dashboardWidget));
    m_currentAccountNumber = accountNumber;
    
    QVariantMap accountDetails = m_bankModel->getAccountDetails(accountNumber);
    QVariantList transactions = m_bankModel->getTransactions(accountNumber);
    
    m_accountWidget->updateAccountDetails(accountDetails);
    m_accountWidget->updateTransactionList(transactions);
    
    m_stackedWidget->setCurrentWidget(m_accountWidget);
}

void MainWindow::showTransactionScreen(const QString &accountNumber, const QString &transactionType)
{
    m_screenStack.push(m_stackedWidget->currentIndex());
    m_currentAccountNumber = accountNumber;
    m_currentTransactionType = transactionType;
    
    QVariantMap accountDetails = m_bankModel->getAccountDetails(accountNumber);
    m_transactionWidget->setupForTransaction(transactionType, accountDetails);
    
    m_stackedWidget->setCurrentWidget(m_transactionWidget);
}

void MainWindow::showVerificationScreen()
{
    m_screenStack.push(m_stackedWidget->currentIndex());
    m_verificationWidget->setupForTransaction(m_currentTransactionType);
    m_stackedWidget->setCurrentWidget(m_verificationWidget);
}

void MainWindow::showCompletionScreen()
{
    m_screenStack.clear(); // 완료 후에는 이전 화면 스택을 비움
    m_completionWidget->setupForTransaction(m_currentTransactionType, m_currentTransactionAmount);
    m_stackedWidget->setCurrentWidget(m_completionWidget);
}

void MainWindow::onLoginSuccess(const QString &username)
{
    showDashboardScreen();
}

void MainWindow::onLogout()
{
    m_bankModel->logout();
    showLoginScreen();
}

void MainWindow::onAccountSelected(const QString &accountNumber)
{
    showAccountScreen(accountNumber);
}

void MainWindow::onTransactionRequested(const QString &accountNumber, const QString &transactionType)
{
    showTransactionScreen(accountNumber, transactionType);
}

void MainWindow::onTransactionAmountEntered(double amount)
{
    m_currentTransactionAmount = amount;
    
    if (m_currentTransactionType == "transfer") {
        // 송금의 경우 계좌번호 입력 화면으로 이동
        m_transactionWidget->showTargetAccountInput();
    } else {
        // 입금/출금의 경우 바로 인증 화면으로 이동
        showVerificationScreen();
    }
}

void MainWindow::onTransactionTargetEntered(const QString &targetAccount)
{
    m_currentTargetAccount = targetAccount;
    showVerificationScreen();
}

void MainWindow::onVerificationCompleted(const QString &verificationCode)
{
    bool success = false;
    
    if (m_currentTransactionType == "deposit") {
        success = m_bankModel->deposit(m_currentAccountNumber, m_currentTransactionAmount, verificationCode);
    } else if (m_currentTransactionType == "withdraw") {
        success = m_bankModel->withdraw(m_currentAccountNumber, m_currentTransactionAmount, verificationCode);
    } else if (m_currentTransactionType == "transfer") {
        success = m_bankModel->transfer(m_currentAccountNumber, m_currentTargetAccount, m_currentTransactionAmount, verificationCode);
    }
    
    if (success) {
        showCompletionScreen();
    } else {
        QMessageBox::warning(this, "거래 오류", "거래를 처리할 수 없습니다. 입력 정보를 확인해주세요.");
    }
}

void MainWindow::onTransactionCompleted()
{
    showAccountScreen(m_currentAccountNumber);
}

void MainWindow::onBackRequested()
{
    if (!m_screenStack.isEmpty()) {
        int previousScreen = m_screenStack.pop();
        m_stackedWidget->setCurrentIndex(previousScreen);
    } else {
        showDashboardScreen(); // 안전 장치
    }
}
