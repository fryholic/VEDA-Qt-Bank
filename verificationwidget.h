#ifndef VERIFICATIONWIDGET_H
#define VERIFICATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class VerificationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit VerificationWidget(QWidget *parent = nullptr);
    
    void setupForTransaction(const QString &transactionType);
    
signals:
    void backRequested();
    void verificationCompleted(const QString &verificationCode);
    
private slots:
    void onNextButtonClicked();
    
private:
    QString m_transactionType;
    
    QLabel *m_titleLabel;
    QLineEdit *m_verificationEdit;
    QPushButton *m_backButton;
    QPushButton *m_nextButton;
};

#endif // VERIFICATIONWIDGET_H
