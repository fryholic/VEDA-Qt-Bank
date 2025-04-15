#ifndef COMPLETIONWIDGET_H
#define COMPLETIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMovie>

class CompletionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompletionWidget(QWidget *parent = nullptr);
    
    void setupForTransaction(const QString &transactionType, double amount);
    
signals:
    void backToAccountRequested();
    
private:
    QLabel *m_completeTextLabel;
    QLabel *m_amountLabel;
    QLabel *m_iconLabel;
    QMovie *m_movie;
};

#endif // COMPLETIONWIDGET_H
